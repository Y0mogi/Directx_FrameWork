#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"

#include "input.h"
#include "modelRenderer.h"


#include "title.h"
#include "game.h"
#include "result.h"

#include "audio.h"

Scene* Manager::m_NowScene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
    // レンダラーの初期化
    Renderer::Init();

    Input::Init();

    Audio::InitMaster();

    m_NowScene = new Title();
    m_NowScene->Init();

}

void Manager::Uninit()
{
    m_NowScene->Uninit();
    delete m_NowScene;

    // ImGuiの終了処理
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    ModelRenderer::UnloadAll();

    Audio::UninitMaster();

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
    ImGui::SetWindowFontScale(1.2f);


    m_NowScene->Update();

    // ImGui更新終了
    ImGui::End();
    ImGui::EndFrame();

    if (m_NextScene) {
        if (m_NowScene) {
            m_NowScene->Uninit();
            delete m_NowScene;
        }

        ModelRenderer::UnloadAll();

        m_NowScene = m_NextScene;
        m_NowScene->Init();

        m_NextScene = nullptr;
    }
}

void Manager::Draw()
{
    // 描画開始
    Renderer::Begin();
   
    m_NowScene->Draw();

    // ImGui描画
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // 描画終了
    Renderer::End();
}
