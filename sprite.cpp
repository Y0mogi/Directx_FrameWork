#include "main.h"
#include "renderer.h"

#include "transform.h"
#include "sprite.h"
#include "gameobject.h"


void Sprite::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	VERTEX_3D vertex[4]{};

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	this->CreateTexture(_path);
}

void Sprite::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Sprite::Update()
{

}

void Sprite::Draw()
{
	XMFLOAT3 pos = Parent->GetComponent<Transform>()->position;
	XMFLOAT3 size = Parent->GetComponent<Transform>()->scale;

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	
	// ���_�O�ԁi����̒��_�j
	vertex[0].Position = XMFLOAT3(pos.x - (size.x / 2.0f), pos.y - (size.y / 2.0f), 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = m_Color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	// ���_�P�ԁi�E��̒��_�j
	vertex[1].Position = XMFLOAT3(pos.x + (size.x / 2.0f), pos.y - (size.y / 2.0f), 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = m_Color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	// ���_�Q�ԁi�����̒��_�j
	vertex[2].Position = XMFLOAT3(pos.x - (size.x / 2.0f), pos.y + (size.y / 2.0f), 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = m_Color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	
	// ���_�R�ԁi�E���̒��_�j
	vertex[3].Position = XMFLOAT3(pos.x + (size.x / 2.0f), pos.y + (size.y / 2.0f), 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = m_Color;
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

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

	// material�ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = {1,1,1,1};
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

void Sprite::CompInfo()
{
	using namespace ImGui;
	SeparatorText("SpriteComponent");
	
	if (TreeNode("SPInfo")) {
		// �F�ύX
		SeparatorText("TextureColor");
		ColorEdit4("Color", reinterpret_cast<float*>(&this->m_Color), ImGuiColorEditFlags_Uint8);

		// �t�@�C���p�X�\���E�ύX
		SeparatorText("FilePath");
		static std::string tmp = utf8_encode(_path);
		TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Do not specify the wrong path!");
		InputText(" ", &tmp); SameLine();
		if (Button("Change")) { if (SaveFilePath(utf8_decode(tmp))) this->CreateTexture(_path); };

		// ���摜�v���r���[
		SeparatorText("DrawRawImage");
		ImGui::Image(m_Texture, ImVec2(200.f, 200.f));

		TreePop();
	}
}

void Sprite::SetColor(const XMFLOAT4& color)
{
	m_Color = color;
}

void Sprite::LoadTexture(const std::wstring& path)
{
	SaveFilePath(path);
}

void Sprite::CreateTexture(const std::wstring& path){

	if (m_Texture) m_Texture->Release(); // 

	// �e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &metadata, image);

	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);
}

bool Sprite::SaveFilePath(const std::wstring& path)
{
	if (_path == path) return false;// �p�X�������Ȃ�ύX���Ȃ�
	_path = path;
	return true;
}
