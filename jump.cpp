#include "main.h"
#include "renderer.h"
#include "gameobject.h"
#include "transform.h"
#include "input.h"
#include "jump.h"

XMFLOAT2 Jump::_accel = { 0.0f,-20.f };

void Jump::Init()
{
	return;
}

void Jump::Uninit()
{
	return;
}

void Jump::Update()
{
	static XMFLOAT3 spos{};
	static float dt = 1.0f / 60.0f;
	
	if (Input::GetKeyTrigger('V'))
	{
		spos = Parent->GetComponent<Transform>()->position;
		_dir.y = 10.f;
	}

	_dir.y += _accel.y * dt;
	Parent->GetComponent<Transform>()->position += _dir * dt;

	if (spos.y > Parent->GetComponent<Transform>()->position.y)
		Parent->GetComponent<Transform>()->position.y = 0.0f;
}

void Jump::Draw()
{
	return;
}
