#pragma once

// #1 Window provide
#include <cassert>
#include <cmath>

#include <vector>
using std::vector;

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTexd.lib")

// #2 Project provide
#include "Resource.h"

#include "framework.h"

// #3 custom project setting
#include "define.h"
#include "struct.h"
#include "function.h"

// #4 many use class
#include "CCore.h"