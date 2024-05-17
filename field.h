#pragma once
#include "component.h"

class Transform;

class Field :public	Component
{
public:
	void Init(const XMFLOAT3& pos, const XMFLOAT3& scl, const XMFLOAT4& color, const wchar_t* path);
	void Uninit();
	void Update();
	void Draw();
private:
	Transform* _transform;

	ID3D11Buffer* _VertexBuffer = NULL;
	ID3D11ShaderResourceView* _Texture = NULL;

	ID3D11VertexShader* _VertexShader;
	ID3D11PixelShader* _PixelShader;
	ID3D11InputLayout* _VertexLayout;
};



