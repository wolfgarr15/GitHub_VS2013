////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////

////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once
#define WIN32_LEAN_AND_MEAN

///////////////////////////////
// Includes
//-----------------------------
#include <Windows.h>
#include <fstream>
#include "d3dclass.h"

///////////////////////////////
// Globals
//-----------------------------
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

///////////////////////////////
// Class Declaration
//-----------------------------
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& src);
	~GraphicsClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D;
};

