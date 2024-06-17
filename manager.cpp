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
