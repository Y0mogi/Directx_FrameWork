#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include <string>

void Polygon2D::Init()
{
	XMFLOAT3 pos = _transform.position;
	XMFLOAT2 size = { _transform.scale.x,_transform.scale.y };

	VERTEX_3D vertex[4];
	// ���_�O�ԁi����̒��_�j
	vertex[0].Position = XMFLOAT3(pos.x - (size.x / 2.0f),pos.y - (size.y / 2.0f), 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = _color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	// ���_�P�ԁi�E��̒��_�j
	vertex[1].Position = XMFLOAT3(pos.x + (size.x / 2.0f), pos.y - (size.y / 2.0f), 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = _color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	// ���_�Q�ԁi�����̒��_�j
	vertex[2].Position = XMFLOAT3(pos.x - (size.x / 2.0f), pos.y + (size.y / 2.0f), 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = _color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	// ���_�R�ԁi�E���̒��_�j
	vertex[3].Position = XMFLOAT3(pos.x + (size.x / 2.0f), pos.y + (size.y / 2.0f), 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = _color;
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(_path, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");


}

void Polygon2D::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Polygon2D::Update()
{
}

void Polygon2D::Draw()
{
	// ���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���b�N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

}

Polygon2D::Polygon2D(const XMFLOAT3& pos, const XMFLOAT2& size, const XMFLOAT4& color, const wchar_t* path)
	:GameObject({ pos, { 0.0f,0.0f,0.0f }, { size.x,size.y,0.0f} })
{
	_color = color;
	_path = _wcsdup(path);
}
