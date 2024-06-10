#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"
#include "input.h"
#include "camera.h"
#include "math_helper.h"


void Camera::Init()
{
	if (_transform == nullptr)
		_transform = Parent->GetComponent<Transform>();
	_transform->position = {0.0f,5.0f,-10.0f};

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = _transform->position.x - _target.x;
	vz = _transform->position.z - _target.z;
	_length = sqrtf(vx * vx + vz * vz);
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	// クオータニオンでの回転

	if (Input::GetKeyPress('Q')) {// 視点旋回「左」
		// クォータニオンを作成して、Y軸を中心に逆方向に回転する
		Quaternion rotationDelta = Quaternion::AngleAxis((float)(M_PI * 0.01), _transform->GetTop());
		// 現在の回転に乗じる
		_transform->rotation = rotationDelta * _transform->rotation;
	}


	
	if (Input::GetKeyPress('E')) {// 視点旋回「右」
		// クォータニオンを作成して、Y軸を中心に逆方向に回転する
		Quaternion rotationDelta = Quaternion::AngleAxis((float)(-M_PI * 0.01), _transform->GetTop());
		// 現在の回転に乗じる
		_transform->rotation = rotationDelta * _transform->rotation;

	}

	// カメラを初期に戻す
	if (Input::GetKeyPress('P'))
	{
		Uninit();
		Init();
	}

	UpdatePosition();

	ImGui::Text("camera Rotation E:x.%f y.%f z.%f", _transform->rotation.ToXMFloat4().x, _transform->rotation.ToXMFloat4().y, _transform->rotation.ToXMFloat4().z);
}

void Camera::Draw()
{
	// ビューマトリックス設定
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	XMMATRIX viewMatrix = XMMatrixLookAtLH(
		XMLoadFloat3(&_transform->position),
		XMLoadFloat3(&_target),
		XMLoadFloat3(&up)
	);

	Renderer::SetViewMatrix(viewMatrix);
	XMStoreFloat4x4(&_viewMatrix, viewMatrix);

	// プロジェクションマトリックス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);

}

void Camera::UpdatePosition()
{
	XMFLOAT3 forward = _transform->GetForward();
	_transform->position.x = _target.x - forward.x * _length;
	_transform->position.z = _target.z - forward.z * _length;
}
