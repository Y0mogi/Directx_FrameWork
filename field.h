#pragma once
#include "component.h"

class Transform;

class Field :public	Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	Field() {};
	Field(const XMFLOAT4& color, const wchar_t* path);
	~Field() override {};

private:
	XMFLOAT4 m_Color{};
	const wchar_t* m_Path{};

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
};



