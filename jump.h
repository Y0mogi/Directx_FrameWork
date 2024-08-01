#pragma once
#ifndef JUMP_H
#define JUMP_H

#include "component.h"

class Jump : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update(const float& dt) override;
	void Draw() override;

private:
	Transform* _transform = nullptr;
	XMFLOAT2 _accel{}; // �����x
	XMFLOAT3 _dir{}; // �����x�N�g��
	bool	_isJump;
};

#endif // !JUMP_H