#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "component.h"

class Camera : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	inline void SetTarget(const XMFLOAT3& target) { this->_target = target; }

private:

	Transform* _transform = nullptr;
	XMFLOAT3 _target{};
	float _length;
	XMFLOAT4X4 _viewMatrix{};
};

#endif // CAMERA_H