#pragma once
#include "component.h"

class Sprite : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	Sprite(const XMFLOAT4& color);

private:
	Transform* m_Transfrom = nullptr;
	XMFLOAT4 m_Color{};

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
};


