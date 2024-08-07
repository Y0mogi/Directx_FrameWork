#pragma once
#include "component.h"

class missiletest : public Component
{
public:
public:
	void Init() override;
	void Uninit() override;
	void Update(const float& dt) override;
	void Draw() override;

	missiletest(GameObject* target)
		:m_Target(target){};
	missiletest() {};
	~missiletest() override {};

private:
	class GameObject* m_Target{};
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	int m_Speed{};
	XMFLOAT3 m_Intpos;
};

