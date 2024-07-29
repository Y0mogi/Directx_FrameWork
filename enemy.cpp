#include "main.h"
#include "renderer.h"

#include "modelRenderer.h"
#include "input.h"
#include "gameobject.h"
#include "transform.h"
#include "enemy.h"
#include "math_helper.h"


void Enemy::Init()
{
	Parent->GetComponent<ModelRenderer>()->Load("asset\\model\\nonderiRobo.obj");
	Parent->GetComponent<Transform>()->position.x += 20;

	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\unlitTexturePS.cso");

}

void Enemy::Uninit()
{
	_VertexShader->Release();
	_VertexLayout->Release();
	_PixelShader->Release();
}

void Enemy::Update()
{
	using namespace DirectX::SimpleMath;
	static auto transform = Parent->GetComponent<Transform>();

	//if (Input::GetKeyPress(VK_LEFT))
	//{
	//	Parent->GetComponent<Transform>()->position.x -= 0.1f;
	//}
	//
	//if (Input::GetKeyPress(VK_RIGHT))
	//{
	//	Parent->GetComponent<Transform>()->position.x += 0.1f;
	//}
	//
	//if (Input::GetKeyPress(VK_UP))
	//{
	//	Parent->GetComponent<Transform>()->position.z += 0.1f;
	//}
	//
	//if (Input::GetKeyPress(VK_DOWN))
	//{
	//	Parent->GetComponent<Transform>()->position.z -= 0.1f;
	//}
	//

	if (Input::GetKeyPress('J'))
	{
		Parent->GetComponent<Transform>()->position.x -= 0.1f;
	}

	if (Input::GetKeyPress('L'))
	{
		Parent->GetComponent<Transform>()->position.x += 0.1f;
	}

	if (Input::GetKeyPress('I'))
	{
		Parent->GetComponent<Transform>()->position.z += 0.1f;
	}

	if (Input::GetKeyPress('K'))
	{
		Parent->GetComponent<Transform>()->position.z -= 0.1f;
	}

	if (Input::GetKeyTrigger('G')) { Parent->Discard(); }
}

void Enemy::Draw()
{
	using namespace DirectX::SimpleMath;
	// 入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	XMMATRIX world, scl, rot, trans;
	scl = DirectX::XMMatrixScaling(Parent->GetComponent<Transform>()->scale.x, Parent->GetComponent<Transform>()->scale.y, Parent->GetComponent<Transform>()->scale.z);
	rot = Matrix::CreateFromQuaternion(Parent->GetComponent<Transform>()->rotation);
	trans = DirectX::XMMatrixTranslation(Parent->GetComponent<Transform>()->position.x, Parent->GetComponent<Transform>()->position.y, Parent->GetComponent<Transform>()->position.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(world);


	Parent->GetComponent<ModelRenderer>()->DrawModel();
}

void Enemy::OnCollisionEnter(const GameObject* collision)
{
	Parent->GetComponent<Transform>()->position.y += 0.1f;
}
