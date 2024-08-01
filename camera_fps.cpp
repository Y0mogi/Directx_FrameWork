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

void Camera_FPS::Update(const float& dt)
{
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
