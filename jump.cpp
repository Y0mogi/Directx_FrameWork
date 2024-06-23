#include "main.h"
#include "renderer.h"
#include "gameobject.h"
#include "transform.h"
#include "input.h"
#include "jump.h"


void Jump::Init()
{
	_transform = Parent->GetComponent<Transform>();
	_accel = { 0.0f,-20.f };
	return;
}

void Jump::Uninit()
{
	return;
}

void Jump::Update()
{
	static float spos{}; // ジャンプの始点
	static float dt = 1.0f / 60.0f; // 媒介変数
	
	if (Input::GetKeyTrigger(VK_SPACE) && !_isJump){
		_isJump = true;
		//spos = _transform->position.y;
		spos = 0;
		_dir.y = 10.f;
	}

	// ジャンプ中のみ処理
	if(_isJump){
		_dir.y += _accel.y * dt; // 速度ベクトルに加速度を加算
		_transform->position += _dir * dt; 
	}

	// ジャンプ開始地点まで降下したらフラグをfalseに
	if (spos >= _transform->position.y) {
		_isJump = false;
		_transform->position.y = spos;
	}

	return;
}

void Jump::Draw()
{
	return;
}
