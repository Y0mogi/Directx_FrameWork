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

	inline void SetTarget(Transform* pTarget) { this->m_Target = pTarget; }

private:
	void UpdatePosition();
private:

	Transform* m_Transform = nullptr;
	Transform* m_Target = nullptr;
	XMFLOAT4X4 m_ViewMatrix{};
	float m_Length, m_Yaw, m_Pitch;

};

#endif // CAMERA_H