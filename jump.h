#pragma once
#ifndef JUMP_H
#define JUMP_H

#include "component.h"

class Jump : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	Transform* _transform = nullptr;
	XMFLOAT2 _accel{}; // 加速度
	XMFLOAT3 _dir{}; // 方向ベクトル
	bool	_isJump;
};

#endif // !JUMP_H