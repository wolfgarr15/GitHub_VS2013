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
#include "modelclass.h"
#include "lightclass.h"
#include "specmapshaderclass.h"

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
	bool Render(float rotation);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightClass* m_Light;
	SpecMapShaderClass* m_SpecMapShader;
};

