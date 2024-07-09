//=============================================================================
// Contents   : カメラコンポーネント
//  
// Author     : 髙橋 脩也
// LastUpdate : 2024/06/26
// Since      : 2024/06/01
//
// === 更新ログ ===
//
// 
//
//
//=============================================================================
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

	inline void SetTarget(Transform* pTarget) { this->_target = pTarget; }

private:
	void UpdatePosition();
private:

	Transform* _transform = nullptr;
	Transform* _target = nullptr;
	XMFLOAT4X4 _viewMatrix{};
	float _length, _yaw, _pitch;

};

#endif // CAMERA_H