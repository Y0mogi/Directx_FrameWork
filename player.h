#pragma once
#include "component.h"

class Player :public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:

	ID3D11VertexShader* _VertexShader;
	ID3D11PixelShader* _PixelShader;
	ID3D11InputLayout* _VertexLayout;
};

