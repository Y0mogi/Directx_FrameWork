#include "main.h"
#include "renderer.h"
#include "game.h"

#include "jump.h"
#include "player.h"
#include "enemy.h"
#include "field.h"
#include "camera.h"
#include "sprite.h"
#include "input.h"
#include "modelRenderer.h"
#include "transform.h"
#include "manager.h"
#include "result.h"

#include "particleemitter.h"
#include "particle_flare.h"

#include "audio.h"

void Game::Init()
{
	using enum Layer;

	// ゲームオブジェクトを作成し、コンポーネントを追加
	//AddObjComp<Sprite>("Sprite", Layer_2, Tag::None,
	//	Sprite{
	//	XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f },
	//	L"asset\\texture\\camera.jpg"
	//	}
	//);

	auto a = AddObjComp<ModelRenderer, OrientedBox, Field>(
		"Field",
		Layer_1,
		Tag::Ground,
		ModelRenderer{},
		OrientedBox{},
		Field{{ 1.0f, 1.0f, 1.0f, 1.0f },L"asset\\texture\\field.png"}
	);
	a->GetComponent<Transform>()->scale = { 100000,0,100000 };
	

	a = AddObjComp<ModelRenderer,Player, OrientedBox, Audio>(
		"Player",
		Layer_1,
		Tag::Player,
		ModelRenderer{},
		Player{},
		OrientedBox{},
		Audio{});
	
	a->GetComponent<Transform>()->scale = { 1,1,1 };
	
	//for (int i = 0; i < 1; i++) {
	//	auto name = std::string("Enemy") + std::to_string(i);
	//	auto b = AddObjComp<ModelRenderer, OrientedBox, Jump, Enemy>
	//		(
	//			name,
	//			Layer_1,
	//			Tag::Enemy,
	//			ModelRenderer{},
	//			OrientedBox{},
	//			Jump{},
	//			Enemy{}
	//		);
	//
	//	b->GetComponent<Transform>()->position = { (5.f * i),1,1 };
	//}
	
	AddObjComp<Camera>("Camera", Layer_0, Tag::None, Camera{});


	//AddObjComp<ParticleEmitter>("Particle", Layer_1, Tag::None,
	//	ParticleEmitter::Factory<Particle_flare>("asset\\texture\\smoke.png",90, { 0.3,0.3,0.3,1 })
	//);
}

void Game::Uninit()
{
	Scene::Uninit();
}

void Game::Update(const float& dt)
{
	Scene::Update(dt);
	if (Input::GetKeyTrigger('T')) Manager::SetScene<Result>();
}

void Game::Draw()
{
	Scene::Draw();
}
