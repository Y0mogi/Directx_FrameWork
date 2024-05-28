#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"
#include "input.h"
#include "camera_fps.h"
#include "math_helper.h"



void Camera_FPS::Init()
{
	if (_transform == nullptr)
		_transform = Parent->GetComponent<Transform>();
}

void Camera_FPS::Uninit()
{

}

void Camera_FPS::Update()
{
	// カメラの注視点をプレイヤーの前方方向にセット
	float pitch = _transform->rotation.x; // カメラの仰角を取得
	float yaw = _transform->rotation.y; // カメラの回転角を取得

	// 前方方向を計算
	XMFLOAT3 forwardVector;
	forwardVector.x = sinf(yaw) * cosf(pitch);
	forwardVector.y = -sinf(pitch);
	forwardVector.z = cosf(yaw) * cosf(pitch);

	// カメラの注視点をプレイヤーの前方方向にセット
	_target = _transform->position + forwardVector;

	//Renderer::DrawDebugData((char*)"a", _target, false, 1, { 500,500 });
}

void Camera_FPS::Draw()
{
	Renderer::SetDepthEnable(true);
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
