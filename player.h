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

	void CompInfo() override;

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader*	m_PixelShader;
	ID3D11InputLayout*	m_VertexLayout;

	int m_Speed{};
	double m_Yaw{}, m_Pitch{}, m_Roll{};	// �p������p
};

#endif // PLAYER_H