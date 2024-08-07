#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "memleakcheck.h"
#include <memory>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#pragma push_macro("new")
#undef new
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DirectX_FrameWork";

#ifdef _DEBUG
int g_CountFPS; // FPSカウンタ
char g_DebugStr[2048] = "DirectX_FrameWork"; // デバッグ文字表示用
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND g_Window;

HWND GetWindow() {
    return g_Window;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // コンソールウィンドウを表示
    AllocConsole();

    // 標準出力と標準エラー出力をコンソールにリダイレクト
    FILE* fp;
    if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0) {
        MessageBoxA(nullptr, "Failed to redirect stdout", "Error", MB_OK);
        return 1;
    }
    if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0) {
        MessageBoxA(nullptr, "Failed to redirect stderr", "Error", MB_OK);
        return 1;
    }
#endif

    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = nullptr;

    if (!RegisterClassEx(&wcex)) {
        return 1; // エラーハンドリング
    }

    RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

    if (!g_Window) {
        return 1; // エラーハンドリング
    }

    CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

    // 初期化
    Manager::Init();

    // ImGuiの初期化
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplWin32_Init(GetWindow());
    ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());
    ImGui::StyleColorsDark();

    ShowWindow(g_Window, nCmdShow);
    UpdateWindow(g_Window);

    DWORD dwExecLastTime = timeGetTime();
    DWORD dwFPSLastTime = dwExecLastTime;
    DWORD dwFrameCount = 0;

    timeBeginPeriod(1);

    MSG msg = {};
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            DWORD dwCurrentTime = timeGetTime();

            if ((dwCurrentTime - dwFPSLastTime) >= 1000) {
#ifdef _DEBUG
                g_CountFPS = dwFrameCount;
#endif
                dwFPSLastTime = dwCurrentTime;
                dwFrameCount = 0;
            }

            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 120)) {
                float deltaTime = (dwCurrentTime - dwExecLastTime) / 1000.0f;

                // 更新
                Manager::Update(deltaTime);

                // 描画
                Manager::Draw();

#ifdef _DEBUG
                wsprintf(g_DebugStr, WINDOW_NAME);
                wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
                SetWindowText(g_Window, g_DebugStr);
#endif

                dwExecLastTime = dwCurrentTime;
                dwFrameCount++;
            }
        }
    }

    timeEndPeriod(1);

    UnregisterClass(CLASS_NAME, wcex.hInstance);

    Manager::Uninit();

    CoUninitialize();

    return _CrtDumpMemoryLeaks();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hWnd);
        }
        break;

        // ImGui用のマウスイベント処理
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_XBUTTONDBLCLK:
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        break;

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring StringToWString(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}
