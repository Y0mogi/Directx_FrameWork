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
	static float spos{}; // �W�����v�̎n�_
	static float dt = 1.0f / 60.0f; // �}��ϐ�
	
	if (Input::GetKeyTrigger(VK_SPACE) && !_isJump){
		_isJump = true;
		//spos = _transform->position.y;
		spos = 0;
		_dir.y = 10.f;
	}

	// �W�����v���̂ݏ���
	if(_isJump){
		_dir.y += _accel.y * dt; // ���x�x�N�g���ɉ����x�����Z
		_transform->position += _dir * dt; 
	}

	// �W�����v�J�n�n�_�܂ō~��������t���O��false��
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
