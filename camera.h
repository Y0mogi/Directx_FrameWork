#pragma once
#include "gameobject.h"

class Camera : public GameObject
{
public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTarget(const XMFLOAT3& target) { this->_target = target; }

	Camera();
	~Camera() override;
private:
	XMFLOAT3 _target{};
	XMFLOAT4X4 _viewMatrix{};
};