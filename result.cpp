#include "main.h"
#include "renderer.h"
#include "result.h"
#include "title.h"
#include "input.h"
#include "manager.h"


void Result::Init()
{

}

void Result::Uninit()
{
	Scene::Uninit();
}

void Result::Update(const float& dt)
{
	Scene::Update(dt);
	if (Input::GetKeyTrigger('T')) Manager::SetScene<Title>();
}

void Result::Draw()
{
	Scene::Draw();
}
