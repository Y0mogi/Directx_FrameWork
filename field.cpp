#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"
#include "field.h"
#include <string>

void Field::Init(const XMFLOAT3& pos, const XMFLOAT3& scl, const XMFLOAT4& color, const wchar_t* path)
{
	if (_transform == nullptr)
		_transform = Parent->GetComponent<Transform>();

	_transform->position = pos;
	_transform->scale = scl;

	VERTEX_3D vertex[4];
	// 頂点０番（左奥の頂点）
	vertex[0].Position = XMFLOAT3(pos.x - (scl.x / 2.0f),pos.y,pos.z + (scl.z / 2.0f));
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	
	// 頂点１番（右奥の頂点）
	vertex[1].Position = XMFLOAT3(pos.x + (scl.x / 2.0f), pos.y, pos.z + (scl.z / 2.0f));
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	
	// 頂点２番（左手前の頂点）
	vertex[2].Position = XMFLOAT3(pos.x - (scl.x / 2.0f), pos.y, pos.z - (scl.z / 2.0f));
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	
	// 頂点３番（右手前の頂点）
	vertex[3].Position = XMFLOAT3(pos.x + (scl.x / 2.0f), pos.y, pos.z - (scl.z / 2.0f));
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_VertexBuffer);

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(path, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &_Texture);
	assert(_Texture);

	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\unlitTexturePS.cso");


}

void Field::Uninit()
{
	_VertexBuffer->Release();
	_Texture->Release();

	_VertexLayout->Release();
	_VertexShader->Release();
	_PixelShader->Release();
}

void Field::Update()
{

}

void Field::Draw()
{
	
	// 入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	// ワールドマトリックス設定
	XMMATRIX world, scl, rot, trans;
	scl = XMMatrixScaling(_transform->scale.x, _transform->scale.y, _transform->scale.z);
	rot = XMMatrixRotationRollPitchYaw(_transform->rotation.x, _transform->rotation.y, _transform->rotation.z);
	trans = XMMatrixTranslation(_transform->position.x, _transform->position.y, _transform->position.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	// material設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}