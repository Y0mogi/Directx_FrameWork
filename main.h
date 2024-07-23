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

// DirectXTK
#include "Src\pch.h"
#include "directxTK_all.h"
#if _DEBUG
#pragma comment(lib,"DirectXTK_Debug.lib")
#else
#pragma comment(lib,"DirectXTK_Release.lib")
#endif

#pragma comment (lib, "winmm.lib")

// Imgui
#include "Imgui\imgui.h"
#include "Imgui\imstb_rectpack.h"
#include "Imgui\imgui_internal.h"
#include "Imgui\imgui_stdlib.h"
#include "Imgui\imgui_impl_dx11.h"
#include "Imgui\imgui_impl_win32.h"

// MagicEnum
#include "magic_enum/magic_enum_all.hpp"

// XMFLOATÉwÉãÉp
#include "XMFLOAT_Helper.h"

#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

#define NULLSEARCH(target) if(!target) assert(0 && "Ç ÇÈÇ€ Å® ÉKÉb");

HWND GetWindow();

void Invoke(std::function<void()> Function, int Time);

std::string WStringToString(const std::wstring& wstr);
std::wstring StringToWString(const std::string& str);