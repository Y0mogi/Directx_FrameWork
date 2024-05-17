#include "manager.h"
#include "main.h"
#include "renderer.h"

#include <list>

#include "polygon2D.h"
#include "field.h"
#include "camera.h"
#include "transform.h"

#include "gameobject.h"

// オブジェクトのリストをグローバルに定義
std::list<GameObject*> Objects{};

void Manager::Init()
{
    // レンダラーの初期化
    Renderer::Init();

    // ゲームオブジェクトを作成し、コンポーネントを追加
    GameObject* obj = new GameObject();
    obj->AddComponent<Transform>();
    obj->AddComponent<Field>();
    Objects.push_back(obj);

    obj = new GameObject();
    obj->AddComponent<Transform>();
    obj->AddComponent<Polygon2D>();
    Objects.push_back(obj);

    obj = new GameObject();
    obj->AddComponent<Transform>();
    obj->AddComponent<Camera>();
    Objects.push_back(obj);

    // 全オブジェクトを初期化
    for (auto& it : Objects)
    {
        // Polygon2D コンポーネントがある場合、その初期化を行う
        if (it->GetComponent<Polygon2D>())
            it->GetComponent<Polygon2D>()->Init(XMFLOAT3{ 100.0f , 100.0f , 0.0f }, XMFLOAT2{ 200.0f , 200.0f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, L"asset\\texture\\camera.jpg");

        // Field コンポーネントがある場合、その初期化を行う
        if (it->GetComponent<Field>())
            it->GetComponent<Field>()->Init(XMFLOAT3{ 0.f,0.f, 0.0f }, XMFLOAT3{ 50.f, 0.0f,50.f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, L"asset\\texture\\field.png");

        // Camera コンポーネントがある場合、その初期化を行う
        if (it->GetComponent<Camera>())
            it->GetComponent<Camera>()->Init();

        // Polygon2D コンポーネントがある場合、それを削除する
        if (it->GetComponent<Polygon2D>())
            it->RemoveComponent<Polygon2D>();
    }

    obj = nullptr;
}

void Manager::Uninit()
{
    // 全オブジェクトの後始末を行う
    for (auto& it : Objects)
    {
        it->Uninit();
    }

    // レンダラーの後始末を行う
    Renderer::Uninit();
}

void Manager::Update()
{
    // 全オブジェクトの更新処理を行う
    for (auto& it : Objects)
    {
        it->Update();
    }
}

void Manager::Draw()
{
    // 描画開始
    Renderer::Begin();

    // 全オブジェクトの描画処理を行う
    for (auto& it : Objects)
    {
        it->Draw();
    }

    // 描画終了
    Renderer::End();
}
