#pragma once
#ifndef PLAYER_H
#define PLAYER_H


#include "component.h"

class Player :public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update(const float& dt) override;
	void Draw() override;

private:

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader*	m_PixelShader;
	ID3D11InputLayout*	m_VertexLayout;
};

#endif // PLAYER_H