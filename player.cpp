#include "main.h"
#include "renderer.h"

#include "modelRenderer.h"
#include "input.h"
#include "gameobject.h"
#include "transform.h"
#include "player.h"
#include "math_helper.h"


void Player::Init()
{
	Parent->GetComponent<ModelRenderer>()->Load("asset\\model\\player.obj");

	Renderer::CreateVertexShader(&_VertexShader, &_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&_PixelShader,
		"shader\\unlitTexturePS.cso");

}

void Player::Uninit()
{
	_VertexShader->Release();
	_VertexLayout->Release();
	_PixelShader->Release();
}

void Player::Update()	
{
	if (Input::GetKeyPress('A'))
	{
		Parent->GetComponent<Transform>()->position.x -= 0.1f;
	}

	if (Input::GetKeyPress('D'))
	{
		Parent->GetComponent<Transform>()->position.x += 0.1f;
	}

	if (Input::GetKeyPress('W'))
	{
		Parent->GetComponent<Transform>()->position.z += 0.1f;
	}

	if (Input::GetKeyPress('S'))
	{
		Parent->GetComponent<Transform>()->position.z -= 0.1f;
	}
	
}

void Player::Draw()
{
	
	// 入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(_PixelShader, NULL, 0);

	XMMATRIX world, scl, rot, trans;
	scl = XMMatrixScaling(Parent->GetComponent<Transform>()->scale.x, Parent->GetComponent<Transform>()->scale.y, Parent->GetComponent<Transform>()->scale.z);
	rot = XMMatrixRotationRollPitchYaw(Parent->GetComponent<Transform>()->rotation.x, Parent->GetComponent<Transform>()->rotation.y, Parent->GetComponent<Transform>()->rotation.z);
	trans = XMMatrixTranslation(Parent->GetComponent<Transform>()->position.x, Parent->GetComponent<Transform>()->position.y, Parent->GetComponent<Transform>()->position.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(world);


	Parent->GetComponent<ModelRenderer>()->DrawModel();
}
