////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "graphicsclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
GraphicsClass::GraphicsClass() 
{
	m_D3D = 0;
	m_Camera = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& src) {}

GraphicsClass::~GraphicsClass() {}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	std::ofstream log;
	char vgaName[256];
	int vgaMemMB;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
		return false;

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass(0.0f, 0.0f, -6.0f);
	if (!m_Camera)
		return false;

	// Create the color shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
		return false;

	// Initialize the color shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
		return false;

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"SomeTexture.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error.", MB_OK);
		return false;
	}

	// Write the VGA info to a log file.
	m_D3D->GetVideoCardInfo(vgaName, vgaMemMB);
	log.open("vgaInfo.log");
	log << "Video Graphics Adapter = " << vgaName << std::endl
		<< "Available Video Memory = " << vgaMemMB << " MB";
	log.close();

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the color shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Shutdown Direct3D.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if(!result)
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX orthoMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z-buffer off to begin 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap on the render pipeline.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if (!result)
		return false;

	// Render the scene using the shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), 
									worldMatrix, viewMatrix, orthoMatrix,                    // <--- NOTE: Use the ortho matrix
									m_Bitmap->GetTexture());							     // in place of the projection matrix for 2D rendering!
	if (!result)
		return false;

	// Turn off the Z-buffer on at the end of 2D rendering.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene.
	m_D3D->EndScene();

	return true;
}
