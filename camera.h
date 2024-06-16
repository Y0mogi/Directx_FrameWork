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

	void CompInfo();

	inline void SetTarget(const XMFLOAT3& target) { this->_target = target; }

private:
	void UpdatePosition();

private:

	Transform* _transform = nullptr;
	XMFLOAT4X4 _viewMatrix{};
	XMFLOAT3 _target{};
	float _length;
	float _yaw, _pitch;   // ヨー角度,ピッチ角度
};

#endif // CAMERA_H