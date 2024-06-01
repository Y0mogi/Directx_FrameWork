#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


#include <DirectXMath.h>
using namespace DirectX;

#include "DirectXTex.h"
#if _DEBUG
#pragma comment(lib,"DirectXTex_Debug.lib")
#else
#pragma comment(lib,"DirectXTex_Release.lib")
#endif


#pragma comment (lib, "winmm.lib")

// Imgui
#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_dx11.h"
#include "Imgui\imgui_impl_win32.h"

// XMFLOAT�w���p
#include "XMFLOAT_Helper.h"

#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

#define DEBUG_DISP_TEXTOUT

HWND GetWindow();

void Invoke(std::function<void()> Function, int Time);

