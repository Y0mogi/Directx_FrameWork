#include "main.h"
#include "renderer.h"

#include "modelRenderer.h"
#include "input.h"
#include "gameobject.h"
#include "transform.h"
#include "player.h"
#include "math_helper.h"

#include "audio.h"
#include "orientedbox.h"

void Player::Init()
{
	Parent->GetComponent<ModelRenderer>()->Load("asset\\model\\fighter.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	Parent->GetComponent<Audio>()->Load("asset\\sound\\lockon000.wav", "lock");

}

void Player::Uninit()
{
	m_VertexShader->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
}

void Player::Update(const float& dt)
{
	using namespace DirectX::SimpleMath;
	auto transform = Parent->GetComponent<Transform>();
	static float speed = 0;

	if (Input::GetKeyPress('A'))
	{
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetForward(), 0.1f);
	}
	
	if (Input::GetKeyPress('D'))
	{
		//Quaternion::Lerp(transform->rotation, Quaternion::CreateFromAxisAngle(transform->GetForward(), -0.1f),dt, transform->rotation);
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetForward(), -0.1f);
	}

	if (Input::GetKeyPress('A') && Input::GetKeyPress('D')) {

		// TODO:普通にミスってる
		// ロールをゼロにするためのクォータニオンを作成
		Quaternion noRoll = Quaternion::CreateFromAxisAngle(transform->GetForward(), 0.0f);

		// 現在の回転とロールがゼロの回転との補間
		transform->rotation = Quaternion::Slerp(transform->rotation, noRoll, dt * 5.0f);
	}
	
	if (Input::GetKeyPress('W'))
	{
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetLeft(), -0.1f);
	}
	
	if (Input::GetKeyPress('S'))
	{
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetLeft(), 0.1f);
	}
	
	
	//if (Input::GetKeyPress('T')) {
	//	transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetForward(), -DirectX::XM_PIDIV4 * 0.1f);
	//}


	// 座標更新
	
	if (Input::GetKeyPress(VK_SHIFT)) {
		speed++;
	}
	else {
		speed--;
	}

	// スピードが早くなりすぎないようにクランプ
	speed = std::clamp<float>(speed,300.f,1000.f);

	transform->position += transform->GetForward() * speed * dt;
}

void Player::Draw()
{
	using namespace DirectX::SimpleMath;
	// 入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMMATRIX world, scl, rot, trans;
	scl = DirectX::XMMatrixScaling(Parent->GetComponent<Transform>()->scale.x, Parent->GetComponent<Transform>()->scale.y, Parent->GetComponent<Transform>()->scale.z);
	rot = Matrix::CreateFromQuaternion(Parent->GetComponent<Transform>()->rotation);
	trans = DirectX::XMMatrixTranslation(Parent->GetComponent<Transform>()->position.x, Parent->GetComponent<Transform>()->position.y, Parent->GetComponent<Transform>()->position.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(world);


	Parent->GetComponent<ModelRenderer>()->DrawModel("asset\\model\\fighter.obj");
}
