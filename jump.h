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
	static XMFLOAT2 _accel; // �����x
	XMFLOAT3 _dir{}; // �����x�N�g��
};

