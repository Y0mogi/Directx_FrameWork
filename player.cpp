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
#include "scene.h"
#include "enemy.h"
#include "particleemitter.h"
#include "particle_flare.h"

#include "missiletest.h"
#include "bullet.h"

#include <iostream>

void Player::Init()
{
	Parent->GetComponent<ModelRenderer>()->Load("asset\\model\\YF23.obj");
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");


	Parent->GetComponent<Audio>()->Load("asset\\sound\\lockon000.wav", "lock");
	
	m_Roll = 1.2;
	m_Pitch = 1.2;
	m_Yaw = 0.5;
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
	
	// 当たり判定用のスケール設定r
	Parent->GetComponent<OrientedBox>()->SetScale({ 2,0.4,6 });

	if (Input::GetKeyPress('A'))
	{
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetForward(), m_Roll * dt);
	}
	
	if (Input::GetKeyPress('D'))
	{
		//Quaternion::Lerp(transform->rotation, Quaternion::CreateFromAxisAngle(transform->GetForward(), -0.1f),dt, transform->rotation);
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetForward(), -m_Roll * dt);
	}

	//if (Input::GetKeyPress('A') && Input::GetKeyPress('D')) {
	//
	//	// TODO:普通にミスってる
	//	// ロールをゼロにするためのクォータニオンを作成
	//	Quaternion noRoll = Quaternion::CreateFromAxisAngle(transform->GetForward(), 0.0f);
	//
	//	// 現在の回転とロールがゼロの回転との補間
	//	transform->rotation = Quaternion::Slerp(transform->rotation, noRoll, dt * 5.0f);
	//}
	
	if (Input::GetKeyPress('W'))
	{
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetLeft(), -m_Pitch * dt);
	}
	
	if (Input::GetKeyPress('S'))
	{
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetLeft(), m_Pitch * dt);
	}

	if (Input::GetKeyPress('Q'))
	{
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetUp(), -m_Yaw * dt);
	}

	if (Input::GetKeyPress('E'))
	{
		transform->rotation *= Quaternion::CreateFromAxisAngle(transform->GetUp(), m_Yaw * dt);
	}

	// 座標更新
	
	if (Input::GetKeyPress(VK_SHIFT)) {
		m_Speed +=10;
	}
	else {
		m_Speed--;
		//std::cout << m_Speed <<std::endl;
	}

	if (Input::GetKeyTrigger(VK_SPACE)) {
		auto tmp = Parent->m_Scene->GetGameObject<Enemy>();
		if (!tmp) { // ロックオンしている
			auto a = Parent->m_Scene->AddObjComp<missiletest>(
				"missile",
				Layer::Layer_1,
				Tag::None,
				missiletest{}
			);
			auto emi = a->AddComponent<ParticleEmitter>(ParticleEmitter::Factory<Particle_flare>("asset\\texture\\smoke.png", 1, { 0.3,0.3,0.3,1 }));
			emi->Init();

			a->GetComponent<Transform>()->position = transform->position;
			a->GetComponent<Transform>()->rotation = transform->rotation;
		}
		else { // ロックオンしていない場合
			auto a = Parent->m_Scene->AddObjComp<missiletest>(
				"missile",
				Layer::Layer_1,
				Tag::None,
				missiletest{tmp}
			);
			auto emi = a->AddComponent<ParticleEmitter>(ParticleEmitter::Factory<Particle_flare>("asset\\texture\\smoke.png", 1, { 0.3,0.3,0.3,1 }));
			emi->Init();

			a->GetComponent<Transform>()->position = transform->position;
			a->GetComponent<Transform>()->rotation = transform->rotation;
		}
	}

	if (Input::GetKeyPress('F')) {
		auto a = Parent->m_Scene->AddObjComp<Bullet>(
			"bullet",
			Layer::Layer_1,
			Tag::None,
			Bullet({1,1,1,1},"asset\\texture\\smoke.png")
		);

		a->GetComponent<Transform>()->position = transform->position;
		a->GetComponent<Transform>()->rotation = transform->rotation;
		a->GetComponent<Transform>()->scale = { 0.2,0.2,0.2};
	}

	//if (speed <= 200) {
	//	transform->rotation = Quaternion::Slerp(transform->rotation, Quaternion::FromToRotation(transform->GetForward(), transform->GetUnder()),dt);
	//}

	// スピードが早くなりすぎないようにクランプ
	m_Speed = std::clamp<int>(m_Speed,100,200);

	transform->position += transform->GetForward() * m_Speed * dt;
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


	Parent->GetComponent<ModelRenderer>()->DrawModel("asset\\model\\YF23.obj");
}

void Player::CompInfo()
{
	using namespace ImGui;
	SeparatorText("playerComponent");

	if (TreeNode("PlayerInfo")) {
		
		DragInt("Speed", &m_Speed, 0, 100000);

		static float tmp[3] =
		{
			m_Yaw,
			m_Pitch,
			m_Roll
		};

		DragFloat3("Yaw:Pitch:Roll", tmp);
		m_Yaw = tmp[0];
		m_Pitch = tmp[1];
		m_Roll = tmp[2];

		TreePop();
	}
}
