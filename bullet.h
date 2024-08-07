#pragma once
#include <string>
#include "component.h"

class Bullet :public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update(const float& dt) override;
	void Draw() override;

	//void CompInfo() override;

	Bullet(const XMFLOAT4 color, std::string path)
		:m_Color(color), m_Path(path){};

private:
	/// <summary>
	/// <para>�e�N�X�`���̓ǂݍ���</para>
	/// �e�N�X�`���̍ēx�����Ȃ̂Ŗ��t���[���̌Ăяo���͋֎~
	/// </summary>
	void CreateTexture(const std::string& path);

	/// <summary>
	/// tmpPath�ւ̃p�X�̕ۑ�
	/// </summary>
	bool SaveFilePath(const std::string& path);
private:
	std::string					m_Path{}; // ���C���p�X
	XMFLOAT4					m_Color = { 1.0f,1.0f,1.0f ,1.0f };

	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};
	ID3D11VertexShader*			m_VertexShader{};
	ID3D11PixelShader*			m_PixelShader{};
	ID3D11InputLayout*			m_VertexLayout{};
};

