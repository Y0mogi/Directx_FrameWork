#pragma once
#include "component.h"

class Jump : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static XMFLOAT2 _accel; // 加速度
	XMFLOAT3 _dir{}; // 方向ベクトル
};

