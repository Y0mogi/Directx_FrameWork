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
	// ���_�O�ԁi�����̒��_�j
	vertex[0].Position = XMFLOAT3(pos.x - (scl.x / 2.0f),pos.y,pos.z + (scl.z / 2.0f));
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	
	// ���_�P�ԁi�E���̒��_�j
	vertex[1].Position = XMFLOAT3(pos.x + (scl.x / 2.0f), pos.y, pos.z + (scl.z / 2.0f));
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	
	// ���_�Q�ԁi����O�̒��_�j
	vertex[2].Position = XMFLOAT3(pos.x - (scl.x / 2.0f), pos.y, pos.z - (scl.z / 2.0f));
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	
	// ���_�R�ԁi�E��O�̒��_�j
	vertex[3].Position = XMFLOAT3(pos.x + (scl.x / 2.0f), pos.y, pos.z - (scl.z / 2.0f));
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &_VertexBuffer);

	// �e�N�X�`���ǂݍ���
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
	
	// ���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	// ���[���h�}�g���b�N�X�ݒ�
	XMMATRIX world, scl, rot, trans;
	scl = XMMatrixScaling(_transform->scale.x, _transform->scale.y, _transform->scale.z);
	rot = XMMatrixRotationRollPitchYaw(_transform->rotation.x, _transform->rotation.y, _transform->rotation.z);
	trans = XMMatrixTranslation(_transform->position.x, _transform->position.y, _transform->position.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(world);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	// material�ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_VertexBuffer, &stride, &offset);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);

}