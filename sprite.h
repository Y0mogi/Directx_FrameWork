#pragma once
#include "component.h"
#include <string>

class Sprite : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetColor(const XMFLOAT4& color);
	void LoadTexture(const std::wstring& path);

private:
	std::wstring _path;
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f ,1.0f };

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
};


