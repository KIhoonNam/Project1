#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")




//////////////
// INCLUDES //
//////////////
#define DIRECTINPUT_VERSION 0x0800
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <directxmath.h>

#include "DDSTextureLoader.h"    // DDS 颇老 贸府
using namespace DirectX;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


///////////////////////////
//  warning C4316 贸府侩  //
///////////////////////////
#include "AlignedAllocationPolicy.h"