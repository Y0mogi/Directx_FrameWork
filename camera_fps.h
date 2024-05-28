#pragma once
#ifndef CAMERA_FPS_H
#define CAMERA_FPS_H

#include "component.h"

class Camera_FPS : public Component
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	Transform* _transform = nullptr;
	XMFLOAT3 _target{};
	float _length;
	XMFLOAT4X4 _viewMatrix{};
};

#endif // CAMERA_FPS_H