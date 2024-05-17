#pragma once
#include "component.h"

class Polygon2D :public Component
{
public:
	void Init(const XMFLOAT3& pos, const XMFLOAT2& size, const XMFLOAT4& color, const wchar_t* path);
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
};

