#pragma once

// #1 Window provide
#include <Windows.h>
#include <windef.h>
#include <WinUser.h>
#include <hidusage.h>

#include <cassert>
#include <cmath>

#include <vector>
using std::vector;

#include <string>
using std::wstring;

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTexd.lib") // DirectX Tex
#pragma comment(lib, "User32.lib") // RegisterRawInputDevices

// #2 Project provide
#include "Resource.h"

#include "framework.h"

// #3 custom project setting
#include "define.h"
#include "struct.h"
#include "function.h"

// #4 many use class
#include "CCore.h"
#include "DirectXTex.h"
using namespace DirectX;
