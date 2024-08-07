#include "main.h"
#include "renderer.h"
#include "bullet.h"

#include "transform.h"
#include "gameobject.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"

void Bullet::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	VERTEX_3D vertex[4]{};

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(StringToWString(m_Path).c_str(), WIC_FLAGS_NONE, &metadata, image);

	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);
}

void Bullet::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Bullet::Update(const float& dt)
{
	static int frame = 0;
	auto transform = Parent->GetComponent<Transform>();

	transform->position += transform->GetForward() * 350 * dt;

	if (frame > 60 * 10) {
		frame = 0;
		Parent->Destroy();
	}
	frame++;
}

void Bullet::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 頂点０番（左奥の頂点）
	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = { 1,1,1,1 };
	vertex[0].TexCoord = XMFLOAT2(0, 0);

	// 頂点１番（右奥の頂点）
	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = { 1,1,1,1 };
	vertex[1].TexCoord = XMFLOAT2(1,0);


	vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = { 1,1,1,1 };
	vertex[2].TexCoord = XMFLOAT2(0,1);

	// 頂点３番（右手前の頂点）
	vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = { 1,1,1,1 };
	vertex[3].TexCoord = XMFLOAT2(1,1);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	/// =================================================-

	// 入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// カメラのマトリックスを取得
	XMMATRIX view = Manager::GetScene()->GetGameObject<Camera>()->GetComponent<Camera>()->GetViewMatrix();

	// viewマトリックスの逆行列をつくる
	XMMATRIX viewInv;
	viewInv = XMMatrixInverse(nullptr, view);
	viewInv.r[3].m128_f32[0] = 0.0f;
	viewInv.r[3].m128_f32[1] = 0.0f;
	viewInv.r[3].m128_f32[2] = 0.0f;

	// ワールドマトリックス設定
	auto transform = Parent->GetComponent<Transform>();
	XMMATRIX world, scl, rot, trans;
	scl = XMMatrixScaling(transform->scale.x, transform->scale.y, transform->scale.z);
	trans = XMMatrixTranslation(transform->position.x, transform->position.y, transform->position.z);
	world = scl * viewInv * trans;
	Renderer::SetWorldMatrix(world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	// material設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::SetDepthEnable(false);
	Renderer::GetDeviceContext()->Draw(4, 0);
	Renderer::SetDepthEnable(true);

}


void Bullet::CreateTexture(const std::string& path) {

	if (m_Texture) m_Texture->Release(); // 

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(StringToWString(path).c_str(), WIC_FLAGS_NONE, &metadata, image);

	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);
}

bool Bullet::SaveFilePath(const std::string& path)
{
	if (m_Path == path) return false;// パスが同じなら変更しない
	m_Path = path;
	return true;
}