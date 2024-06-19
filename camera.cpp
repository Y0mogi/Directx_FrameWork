#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "gameobject.h"

#include "camera.h"
#include "input.h"
#include "scene.h"

using namespace DirectX::SimpleMath;

void Camera::Init()
{
	if (_transform == nullptr) _transform = Parent->GetComponent<Transform>();
		NULLSEARCH(_transform)
	_transform->position = {0.0f,5.0f,-10.0f};

	if (_target == nullptr) _target = Parent->scene->GetGameObject("Player")->GetComponent<Transform>();
	NULLSEARCH(_target)

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = _transform->position.x - _target->position.x;
	vz = _transform->position.z - _target->position.z;
	_length = sqrtf(vx * vx + vz * vz);
}

void Camera::Uninit()
{
	_transform = nullptr;
}

void Camera::Update()
{
	// 回転の速度
	static auto rotationSpeed = XM_PI * 0.01f;

	// 上下左右の回転
	if (Input::GetKeyPress('Q')) {
		_yaw += rotationSpeed;
	}

	if (Input::GetKeyPress('E')) {
		_yaw -= rotationSpeed;
	}

	if (Input::GetKeyPress('W')) {
		_pitch -= rotationSpeed;
	}

	if (Input::GetKeyPress('S')) {
		_pitch += rotationSpeed;
	}

	// カメラを初期に戻す
	if (Input::GetKeyPress('P')) {
		Uninit();
		Init();
	}

	_transform->rotation = Quaternion::CreateFromYawPitchRoll(_yaw, _pitch, 0.0f);

	auto a = BoundingBox();
	auto b = BoundingBox();


	if (a.Intersects(b)) {
		auto b = 1;
	}

	UpdatePosition();

}

void Camera::Draw()
{
	// ビューマトリックス設定
	XMFLOAT3 up = _transform->GetTop();
	XMMATRIX viewMatrix = XMMatrixLookAtLH(
		XMLoadFloat3(&_transform->position),
		XMLoadFloat3(&_target->position),
		XMLoadFloat3(&up)
	);

	Renderer::SetViewMatrix(viewMatrix);
	XMStoreFloat4x4(&_viewMatrix, viewMatrix);
	
	// プロジェクションマトリックス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);

}

void Camera::CompInfo()
{
	using namespace ImGui;
	SeparatorText("Camera");
	if (TreeNode("CAInfo")) {

		auto tmp = _target->position;
		DragFloat3("AtPosition", reinterpret_cast<float*>(&tmp));
		DragFloat("AtDistanth", reinterpret_cast<float*>(&_length));
		TreePop();
	}
}

void Camera::UpdatePosition()
{
	// ターゲット位置からの相対位置を計算
	Vector3 relativePosition = Vector3(_target->position.x, _target->position.y,_length + 0.0001f);
	relativePosition = Vector3::Transform(relativePosition, _transform->rotation);

	// 新しいカメラ位置を計算
	_transform->position = XMFLOAT3(relativePosition);
}
