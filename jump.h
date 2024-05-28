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
	Transform* _transform = nullptr;
	XMFLOAT3 _dir{}; // �����x�N�g��
	bool	_isJump;
};

