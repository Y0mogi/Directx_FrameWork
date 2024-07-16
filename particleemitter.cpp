#include <array>
#include "main.h"
#include "renderer.h"
#include "particleemitter.h"
#include "particle_base.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"

void ParticleEmitter::Init()
{
	VERTEX_3D vertex[4]{};

	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0, 0);

	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1, 0);

	vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0, 1);

	vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1, 1);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(utf8_decode(m_Particles.front()->m_Path).c_str(), WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	
}

void ParticleEmitter::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	for (auto it : m_Particles) {
		delete it;
	}
}

void ParticleEmitter::Update()
{
	for (const auto& it : m_Particles) {
		it->Update();
	}
}

void ParticleEmitter::Draw()
{
	// 入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// カメラのマトリックスを取得
	//XMMATRIX view = Manager

	// viewマトリックスの逆行列をつくる
	XMMATRIX viewInv;
	viewInv = XMMatrixInverse(nullptr, view);
	viewInv.r[3].m128_f32[0] = 0.0f;
	viewInv.r[3].m128_f32[1] = 0.0f;
	viewInv.r[3].m128_f32[2] = 0.0f;

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// material設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4();
	material.TextureEnable = true;
	Renderer::SetMaterial(material);


	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Zバッファ無効
	Renderer::SetDepthEnable(false);

	// 加算合成有効
	//Renderer::SetBlendAddEnable(true);

	for (const auto& it : m_Particles) {
		if (it->GetEnable()) continue;
		XMMATRIX world, scl, trans;
		scl = XMMatrixScaling(it->m_Transform.scale.x,
			it->m_Transform.scale.y,
			it->m_Transform.scale.z);

		trans = XMMatrixTranslation(it->m_Transform.position.x,
			it->m_Transform.position.y,
			it->m_Transform.position.z);

		world = scl * viewInv * trans;

		Renderer::SetWorldMatrix(world);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
		Renderer::GetDeviceContext()->Draw(4, 0);
	}

	Renderer::SetDepthEnable(true);
	//Renderer::SetBlendAddEnable(false);

}
