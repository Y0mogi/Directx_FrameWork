#ifdef _DEBUG
#pragma push_macro("new")
#undef new
#endif

#ifdef _DEBUG
#pragma pop_macro("new")
#endif


#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "memleakcheck.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DirectX_FrameWork";

#ifdef _DEBUG
int		g_CountFPS;								// FPSカウンタ
char	g_DebugStr[2048] = "DirectX_FrameWork";	// デバッグ文字表示用
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ImGuiのDX11ウィンドウプロシージャハンドラの宣言
extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


HWND g_Window;


HWND GetWindow()
{
	return g_Window;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// メモリリークチェックを有効にする
	#ifdef _DEBUG
	EnableMemoryLeakCheck();
	#endif
	
	WNDCLASSEX wcex;
	{
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

		RegisterClassEx(&wcex);
		

		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	// 初期化
	Manager::Init();


	// ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());
	// フォントの設定
	ImGui::StyleColorsDark(); // ダークテーマを使用


	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);


	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	MSG msg;
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif
				dwExecLastTime = dwCurrentTime;
				

				// 更新
				Manager::Update();

				// 描画
				Manager::Draw();

#ifdef _DEBUG
				SetWindowText(g_Window, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();

	return _CrtDumpMemoryLeaks();
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

		#ifdef _DEBUG
		_CrtDumpMemoryLeaks();
		#endif

		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	// Imgui用
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
	///

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



// Convert a wide Unicode string to an UTF8 string
std::string WStringToString(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring StringToWString(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

