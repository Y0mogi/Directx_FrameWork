#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"
#include <string>
#include "field.h"


void Field::Init()
{
	VERTEX_3D vertex[4];

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(m_Path, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");


}

void Field::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Field::Update(const float& dt)
{

}

void Field::Draw()
{
	Transform* tramsform = Parent->GetComponent<Transform>();

	D3D11_MAPPED_SUBRESOURCE msr{};
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	auto pos = tramsform->position;

	// ���_�O�ԁi�����̒��_�j
	vertex[0].Position = XMFLOAT3(-1, pos.y, 1);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = { 1,1,1,1 };
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	// ���_�P�ԁi�E���̒��_�j
	vertex[1].Position = XMFLOAT3(1, pos.y, 1);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = { 1,1,1,1 };
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	// ���_�Q�ԁi����O�̒��_�j
	vertex[2].Position = XMFLOAT3(-1, pos.y, -1);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = { 1,1,1,1 };
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	// ���_�R�ԁi�E��O�̒��_�j
	vertex[3].Position = XMFLOAT3(1, pos.y, -1);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = { 1,1,1,1 };
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer,0);

	// ���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���[���h�}�g���b�N�X�ݒ�
	XMMATRIX world, scl, rot, trans;
	scl = XMMatrixScaling(tramsform->scale.x, tramsform->scale.y, tramsform->scale.z);
	rot = XMMatrixRotationQuaternion(tramsform->rotation);
	trans = XMMatrixTranslation(tramsform->position.x, tramsform->position.y, tramsform->position.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	// material�ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
 	Renderer::GetDeviceContext()->Draw(4, 0);

}

Field::Field(const XMFLOAT4& color, const wchar_t* path)
{
	m_Color = color;
	m_Path = path;
}
