#pragma once
#include "gameobject.h"

class Polygon2D :public GameObject
{
public:
	void Init()override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	Polygon2D() = delete;
	Polygon2D(const XMFLOAT3& pos, const XMFLOAT2& size, const XMFLOAT4& color,const wchar_t* path);
	~Polygon2D() override { free((void*)_path); };

private:
	XMFLOAT4 _color{};
	const wchar_t* _path{};

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
};

