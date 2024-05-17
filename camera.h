#pragma once
#include "component.h"

class Camera : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetTarget(const XMFLOAT3& target) { this->_target = target; }

private:
	Transform* _transform = nullptr;
	XMFLOAT3 _target{};
	XMFLOAT4X4 _viewMatrix{};
};