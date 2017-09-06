// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

using namespace std;
using namespace DirectX;


#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include "DDSTextureLoader.h"
// TODO: reference additional headers your program requires here

//----------------------------------------------------------------------------------------------------------
// Structures:
//----------------------------------------------------------------------------------------------------------
#pragma region
// Vertex Loading
struct SIMPLE_VERTEX {
	XMFLOAT4					points;
	XMFLOAT4					Color;
	XMFLOAT2					uvs;
	XMFLOAT4					normals;
};

// Constant Buffer 1
struct ConstantMatrix
{
	XMMATRIX					World;
	XMMATRIX					View;
	XMMATRIX					Projection;
};

struct Lighting
{
	XMFLOAT4 Direction;
	XMFLOAT4 Color;
	XMFLOAT4 Position;
	XMFLOAT4 Radius;
};
#pragma endregion