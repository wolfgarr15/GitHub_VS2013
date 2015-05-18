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
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "modellist.h"
#include "frustumclass.h"

///////////////////////////////
// Globals
//-----------------------------
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
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
	bool Frame(int FPS, float rotationY);
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextClass* m_Text;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	ModelList* m_ModelList;
	FrustumClass* m_Frustum;
};

