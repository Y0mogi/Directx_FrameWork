#pragma once
#include "gameobject.h"

class Field :public	GameObject
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	Field() = delete;
	Field(const XMFLOAT3& pos, const XMFLOAT3& scl, const XMFLOAT4& color, const wchar_t* path);
	~Field() override  { free((void*)_path); };

	;
private:
	XMFLOAT4 _color{};
	const wchar_t* _path{};

	ID3D11Buffer* _VertexBuffer = NULL;
	ID3D11ShaderResourceView* _Texture = NULL;

	ID3D11VertexShader* _VertexShader;
	ID3D11PixelShader* _PixelShader;
	ID3D11InputLayout* _VertexLayout;
};



