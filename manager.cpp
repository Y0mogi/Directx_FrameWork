#include "manager.h"
#include "main.h"
#include "renderer.h"
#include "input.h"

#include <list>

#include "gameobject.h"

#include "sprite.h"
#include "field.h"
#include "camera.h"
#include "camera_fps.h"
#include "player.h"
#include "transform.h"
#include "jump.h"
#include "modelRenderer.h"
#include "scene.h"

float screenX;
float screenY;
float screenWidth;
float screenHeight;

Scene* _scene{};

void Manager::Init()
{
    // レンダラーの初期化
    Renderer::Init();

    Input::Init();

    _scene = new Scene();
    _scene->Init();

}

void Manager::Uninit()
{

    // ImGuiの終了処理
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    _scene->Uninit();

    Input::Uninit();

    // レンダラーの後始末を行う
    Renderer::Uninit();
}

void Manager::Update()
{
    Input::Update();

    // ImGuiの更新開始
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("DebugWindow");
    ImGui::SetWindowFontScale(1.2);

    _scene->Update();

    /*
    XMFLOAT3 pos;
    XMFLOAT3 size;

    // 全オブジェクトの更新処理を行う
    for (auto& it : Objects)
    {
        
        if (it->GetComponent<Player>()) {
            pos = it->GetComponent<Transform>()->position;
            size = it->GetComponent<Transform>()->scale;
        }
    
    }

    

    // 3Dオブジェクトのバウンディングボックスの座標（例として仮定）
    float minX = -size.x;
    float minY = -size.y;
    float minZ = -size.z;
    float maxX = size.x;
    float maxY = size.y;
    float maxZ = size.z;

    // 入力パラメータ
    XMMATRIX worldMatrix = XMMatrixIdentity();     // ワールド行列
    XMMATRIX viewMatrix = XMMatrixIdentity();      // ビュー行列
    XMMATRIX projectionMatrix = XMMatrixIdentity();// プロジェクション行列
    XMVECTOR objPos = ToXMVECTOR(pos);
    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, 800.0f, 600.0f, 0.0f, 1.0f };  // ビューポート

    // 1. モデル座標からワールド座標に変換
    XMVECTOR worldPos = XMVector3Transform(objPos, worldMatrix);

    // 2. ワールド座標からビュー座標に変換
    XMVECTOR viewPos = XMVector3Transform(worldPos, viewMatrix);

    // 3. ビュー座標からクリップ座標に変換
    XMVECTOR clipPos = XMVector3Transform(viewPos, projectionMatrix);

    // 4. クリップ座標からNDC（正規化デバイス座標）に変換
    XMVECTOR ndcPos = XMVectorDivide(clipPos, XMVectorSplatW(clipPos));

    // 5. NDCからスクリーン座標に変換
    screenX = (ndcPos.m128_f32[0] * 0.5f + 0.5f) * viewport.Width + viewport.TopLeftX;
    screenY = (-ndcPos.m128_f32[1] * 0.5f + 0.5f) * viewport.Height + viewport.TopLeftY;


    //=============== バウンディングボックスの8つの頂点をワールド座標で定義 ===============
    XMVECTOR boundingBox[8] = {
        XMVectorSet(minX, minY, minZ, 1.0f),
        XMVectorSet(maxX, minY, minZ, 1.0f),
        XMVectorSet(minX, maxY, minZ, 1.0f),
        XMVectorSet(maxX, maxY, minZ, 1.0f),
        XMVectorSet(minX, minY, maxZ, 1.0f),
        XMVectorSet(maxX, minY, maxZ, 1.0f),
        XMVectorSet(minX, maxY, maxZ, 1.0f),
        XMVectorSet(maxX, maxY, maxZ, 1.0f)
    };

    // ワールド・ビュー・プロジェクション行列の計算
    XMMATRIX worldViewProjMatrix = worldMatrix * viewMatrix * projectionMatrix;

    // スクリーン座標の最小値と最大値を初期化
    float minScreenX = FLT_MAX;
    float minScreenY = FLT_MAX;
    float maxScreenX = -FLT_MAX;
    float maxScreenY = -FLT_MAX;

    for (int i = 0; i < 8; ++i) {
        // バウンディングボックスの各頂点をスクリーン座標に変換
        XMVECTOR screenPos = XMVector3TransformCoord(boundingBox[i], worldViewProjMatrix);
        float x = (screenPos.m128_f32[0] * 0.5f + 0.5f) * viewport.Width + viewport.TopLeftX;
        float y = (-screenPos.m128_f32[1] * 0.5f + 0.5f) * viewport.Height + viewport.TopLeftY;

        // スクリーン座標の最小値と最大値を更新
        if (x < minScreenX) minScreenX = x;
        if (y < minScreenY) minScreenY = y;
        if (x > maxScreenX) maxScreenX = x;
        if (y > maxScreenY) maxScreenY = y;
    }

    // スクリーン上の大きさを計算
    screenWidth = maxScreenX - minScreenX;
    screenHeight = maxScreenY - minScreenY;


    ImGui::Text("screenPos:x.%f y.%f", screenX, screenY);
    ImGui::Text("screenSize:x.%f y.%f", screenWidth, screenHeight);
    */

    ImGui::End();
    ImGui::EndFrame();
}

void Manager::Draw()
{
    // 描画開始
    Renderer::Begin();


    _scene->Draw();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // 描画終了
    Renderer::End();
}
