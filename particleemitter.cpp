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
	LoadFromWICFile(StringToWString(m_Path).c_str(), WIC_FLAGS_NONE, &metadata, image);
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
}

void ParticleEmitter::Update()
{
	int tmp = m_Count;
	for (const auto& it : m_Particles) {
		it->Update();

		tmp--;
		if (tmp < 0) break;
	}

	for (size_t i = m_Count; i < m_Particles.size(); i++) {
		m_Particles[i]->GetEnable() = false;
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
	XMMATRIX view = Manager::GetScene()->GetGameObject<Camera>()->GetComponent<Camera>()->GetViewMatrix();

	// view�}�g���b�N�X�̋t�s�������
	XMMATRIX viewInv{};
	viewInv = XMMatrixInverse(nullptr, view);
	viewInv.r[3].m128_f32[0] = 0.0f;
	viewInv.r[3].m128_f32[1] = 0.0f;
	viewInv.r[3].m128_f32[2] = 0.0f;

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Z�o�b�t�@����
	Renderer::SetDepthEnable(false);

	// ���Z�����L��
	Renderer::SetBlendAddEnable(true);

	for (const auto& it : m_Particles) {
		if (!it->GetEnable()) continue;

		// material�ݒ�
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = it->GetColor();
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		XMMATRIX world, scl, trans;
		scl = XMMatrixScaling(
			it->GetTransform().scale.x,
			it->GetTransform().scale.y,
			it->GetTransform().scale.z);

		trans = XMMatrixTranslation(
			it->GetTransform().position.x,
			it->GetTransform().position.y,
			it->GetTransform().position.z);

		world = scl * viewInv * trans;

		Renderer::SetWorldMatrix(world);

		// �|���S���`��
		Renderer::GetDeviceContext()->Draw(4, 0);
		Renderer::GetDeviceContext()->Draw(4, 0);
	}

	Renderer::SetDepthEnable(true);
	Renderer::SetBlendAddEnable(false);

}

void ParticleEmitter::CompInfo()
{
	using namespace ImGui;
	SeparatorText("ParticleEmitter");
	
	if (TreeNode("PEInfo")) {

		// �\�����ύX
		DragInt("ParticleCount", reinterpret_cast<int*>(&m_Count), 1, 0, PARTICLE_MAX);
		

		// �Ƃ肠����������
		static XMFLOAT4 tmpColor{ 1,1,1,1 }; 
		// �G�~�b�^�[����̐F�ύX
		ImGui::ColorEdit4("picker", reinterpret_cast<float*>(&tmpColor), ImGuiColorEditFlags_Uint8);
		for (auto &it : m_Particles) {
			if (it->GetEnable()) { it->GetColor() = tmpColor; }
		}
		
		
		// �e�[�u���̕\��
		static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn;
		static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;
		ImGui::NewLine();
		Text("ParticlesTable");
		if (BeginTable("ParticlesTable", 5, table_flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 12)))
		{
			TableSetupScrollFreeze(0, 1);
			TableSetupColumn("ID", ImGuiTableColumnFlags_NoHide | column_flags);
			TableSetupColumn("Enable");
			TableSetupColumn("Position");
			TableSetupColumn("Velocity");
			TableSetupColumn("Color");
			TableHeadersRow();

			for (size_t i = 0; i < m_Particles.size(); ++i)
			{

				TableNextRow();
				TableSetColumnIndex(0);
				Text("%d", static_cast<int>(i)); // �p�[�e�B�N����ID

				TableSetColumnIndex(1);
				auto& enable = m_Particles[i]->GetEnable();
				Text("%s", enable ? "True" : "False"); // �ʒu

				TableSetColumnIndex(2);
				auto& position = m_Particles[i]->GetTransform().position;
				Text("X: %.2f Y: %.2f Z: %.2f", position.x, position.y, position.z); // �ʒu	


				TableSetColumnIndex(3);
				auto& velocity = m_Particles[i]->GetVelocity();
				Text("X: %.2f Y: %.2f Z: %.2f", velocity.x, velocity.y, velocity.z); // ���x

				TableSetColumnIndex(4);
				auto& color = m_Particles[i]->GetColor();
				ColorButton("ColorButton", ImVec4(color.x, color.y, color.z, color.w), ImGuiColorEditFlags_Uint8); // �F���{�^���ŕ\��

			}
			EndTable();
		}


		// ���摜�v���r���[
		ImGui::SeparatorText("DrawRawImage");
		ImGui::Image(m_Texture, ImVec2(200.f, 200.f));

		TreePop();
	}
}
