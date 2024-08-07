#include "main.h"
#include "renderer.h"
#include "billboard_sprite.h"

constexpr auto NUM_VERTEX = 4;

ID3D11Buffer* billboard_sprite::m_VertexBuffer{};

void billboard_sprite::Init()
{
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

}

void billboard_sprite::Uninit()
{
	m_VertexBuffer->Release();
}

void billboard_sprite::DrawBillboard(int texNo, const XMFLOAT3& pos, float Width, float Height, float U, float V, float UW, float VH)
{

}
