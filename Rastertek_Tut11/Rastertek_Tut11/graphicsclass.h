////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////

////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <fstream>
#include "d3dclass.h"
#include "cameraclass.h"
#include "textclass.h"

///////////////////////////////
// Globals
//-----------------------------
const bool FULL_SCREEN = true;
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
	bool Frame(int mouseX, int mouseY);
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextClass* m_Text;
};

