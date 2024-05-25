#include "manager.h"
#include "main.h"
#include "renderer.h"
#include "input.h"

#include <list>

#include "gameobject.h"

#include "sprite.h"
#include "field.h"
#include "camera.h"
#include "player.h"
#include "transform.h"
#include "jump.h"
#include "modelRenderer.h"



// オブジェクトのリストをグローバルに定義
std::list<GameObject*> Objects{};

void Manager::Init()
{
    // レンダラーの初期化
    Renderer::Init();

    Input::Init();

    // ゲームオブジェクトを作成し、コンポーネントを追加
    GameObject* obj = new GameObject();
    obj->AddComponent<Field>();
    Objects.push_back(obj);

    obj = new GameObject();
    obj->AddComponent<ModelRenderer>();
    obj->AddComponent<Jump>();
    obj->AddComponent<Player>();
    Objects.push_back(obj);

    obj = new GameObject();
    obj->AddComponent<Sprite>();
    Objects.push_back(obj);

    obj = new GameObject();
    obj->AddComponent<Camera>();
    Objects.push_back(obj);

    // 全オブジェクトを初期化
    for (auto& it : Objects)
    {
        // Polygon2D コンポーネントがある場合、その初期化を行う
        if (it->GetComponent<Sprite>())
        {
            it->GetComponent<Sprite>()->LoadTexture(L"asset\\texture\\camera.jpg");
            it->GetComponent<Transform>()->position = { 100,100,0 };
            it->GetComponent<Transform>()->scale = { 100,100,0 };
        }

        // Field コンポーネントがある場合、その初期化を行う
        if (it->GetComponent<Field>())
            it->GetComponent<Field>()->Init(XMFLOAT3{ 0.f,0.f, 0.0f }, XMFLOAT3{ 5.f, 0.0f,5.f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, L"asset\\texture\\field.png");

        // Camera コンポーネントがある場合、その初期化を行う
        if (it->GetComponent<Camera>())
            it->GetComponent<Camera>()->Init();

        it->Init();
        
        // Polygon2D コンポーネントがある場合、それを削除する
        //if (it->GetComponent<Polygon2D>())
        //    it->RemoveComponent<Polygon2D>();
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

    Input::Uninit();

    // レンダラーの後始末を行う
    Renderer::Uninit();
}

void Manager::Update()
{
    Input::Update();

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
