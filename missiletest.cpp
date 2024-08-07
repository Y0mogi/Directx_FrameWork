#include "main.h"
#include "renderer.h"
#include "gameobject.h"
#include "missiletest.h"

#include "manager.h"
#include "scene.h"
#include "transform.h"
#include "modelRenderer.h"


void missiletest::Init()
{
	Parent->GetComponent<ModelRenderer>()->Load("asset\\model\\AIM9.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Speed = 300;
	Parent->GetComponent<Transform>()->scale = { 2,2,2 };
}

void missiletest::Uninit()
{
	m_Target = nullptr;
	m_VertexShader->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
}

void missiletest::Update(const float& dt)
{
	static unsigned int life = 0;
	using namespace DirectX::SimpleMath;

	// ターゲットが存在しているかの確認
	if (m_Target) {
		if (!Manager::GetScene()->GetGameObjectAlive(m_Target)) {
			m_Target = nullptr;
		}
	}


	if (m_Target) {
		Transform* target = m_Target->GetComponent<Transform>();
		Transform* parent = Parent->GetComponent<Transform>();

		

		// ターゲットへの方向を求める
		auto direction = target->position - parent->position;
		direction = VectorNormalize(direction); // 正規化
		
		parent->rotation *= Quaternion::FromToRotation(parent->GetForward(), direction); // 前方方向のベクトルをdirectionへ回転させる

		// 球面線形補完
		parent->rotation = Quaternion::Slerp(parent->rotation, Quaternion::FromToRotation(parent->GetForward(), direction), 0.1);

		// 簡易的な破壊処理
		if (EasyDistance(target->position, parent->position) < 3.f) {
			Parent->Destroy();
			m_Target->Destroy();
		}
	}

	// 移動処理を回転処理の後に行う
	Parent->GetComponent<Transform>()->position += Parent->GetComponent<Transform>()->GetForward() * m_Speed * dt;

	life++;
	if (life > (120 * 30)) {
		Parent->Destroy();
		life = 0;
	}
}

void missiletest::Draw()
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


	Parent->GetComponent<ModelRenderer>()->DrawModel("asset\\model\\AIM9.obj");
}
