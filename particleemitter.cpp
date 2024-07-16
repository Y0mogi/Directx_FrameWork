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
	// ���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �J�����̃}�g���b�N�X���擾
	//XMMATRIX view = Manager

	// view�}�g���b�N�X�̋t�s�������
	XMMATRIX viewInv;
	viewInv = XMMatrixInverse(nullptr, view);
	viewInv.r[3].m128_f32[0] = 0.0f;
	viewInv.r[3].m128_f32[1] = 0.0f;
	viewInv.r[3].m128_f32[2] = 0.0f;

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// material�ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4();
	material.TextureEnable = true;
	Renderer::SetMaterial(material);


	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Z�o�b�t�@����
	Renderer::SetDepthEnable(false);

	// ���Z�����L��
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

		// �|���S���`��
		Renderer::GetDeviceContext()->Draw(4, 0);
		Renderer::GetDeviceContext()->Draw(4, 0);
	}

	Renderer::SetDepthEnable(true);
	//Renderer::SetBlendAddEnable(false);

}
