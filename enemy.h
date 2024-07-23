#pragma once
#ifndef ENEMY_H
#define ENEMY_H


#include "component.h"
#include "collision_base.h"

class Enemy :public Component, public CollisionEvent
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void OnCollisionEnter(const GameObject* collision);

private:
	ID3D11VertexShader* _VertexShader;
	ID3D11PixelShader* _PixelShader;
	ID3D11InputLayout* _VertexLayout;
};

#endif // ENEMY_H