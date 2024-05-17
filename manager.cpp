#include "manager.h"
#include "main.h"
#include "renderer.h"

#include <memory>
#include <list>

#include "polygon2D.h"
#include "field.h"
#include "camera.h"

//std::unique_ptr<Polygon2D> polygon{};
//std::unique_ptr<Field>	field{};
//std::unique_ptr<Camera> camera{};

std::list<std::unique_ptr<GameObject>> Objects{};

void Manager::Init()
{
	Renderer::Init();

	Objects.push_back(std::make_unique<Field>(
		XMFLOAT3{ 0.f,0.f, 0.0f },
		XMFLOAT3{ 50.f, 0.0f,50.f },
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f },
		L"asset\\texture\\field.png"
	));

	Objects.push_back(
		std::make_unique<Polygon2D>(
		XMFLOAT3{ 100.0f , 100.0f , 0.0f },
		XMFLOAT2{ 200.0f , 200.0f },
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f },
		L"asset\\texture\\camera.jpg"
	));

	Objects.push_back(std::make_unique<Camera>());

	/*
	polygon = std::make_unique<Polygon2D>(
		XMFLOAT3{ 100.0f , 100.0f , 0.0f },
		XMFLOAT2{ 200.0f , 200.0f },
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f },
		L"asset\\texture\\camera.jpg"
	);
	polygon->Init();

	field = std::make_unique<Field>(
		XMFLOAT3{ 0.f,0.f, 0.0f },
		XMFLOAT3{ 50.f, 0.0f,50.f },
		XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f },
		L"asset\\texture\\field.png"
	);

	field->Init();

	camera = std::make_unique<Camera>();
	camera->Init();
	*/

	for (auto& it : Objects)
	{
		it->Init();
	}

}


void Manager::Uninit()
{
	//camera->Uninit();
	//
	//field->Uninit();
	//
	//polygon->Uninit();

	for (auto& it : Objects)
	{
		it->Uninit();
	}
	
	Renderer::Uninit();

	// poly��delete��Y�ꂸ��
}

void Manager::Update()
{
	//polygon->Update();
	//
	//field->Update();
	//
	//camera->Update();

	for (auto& it : Objects)
	{
		it->Update();
	}
}

void Manager::Draw()
{
	Renderer::Begin();
	
	//camera->Draw();
	//
	//field->Draw();
	//
	//polygon->Draw();

	for (auto& it : Objects)
	{
		it->Draw();
	}

	Renderer::End();
}
