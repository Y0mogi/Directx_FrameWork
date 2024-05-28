#pragma once
#ifndef SPRITE_H
#define SPRITE_H


#include "component.h"
#include <string>

/// <summary>
/// 2D�X�v���C�g��\������R���|�[�l���g
/// </summary>
class Sprite : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	/// <summary>
	/// �X�v���C�g�J���[�̐ݒ�
	/// </summary>
	/// <param name="color">�`�掞�̐F</param>
	void SetColor(const XMFLOAT4& color);

	/// <summary>
	/// �e�N�X�`���̓ǂݍ���
	/// </summary>
	/// <param name="path">�摜�ۑ��ꏊ�̑��΃p�X</param>
	void LoadTexture(const std::wstring& path);

private:
	std::wstring _path{};
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f ,1.0f };

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
};

#endif // !SPRITE_H
