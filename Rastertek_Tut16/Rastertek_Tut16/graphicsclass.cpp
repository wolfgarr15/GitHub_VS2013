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
	m_Model = 0;
	m_AlphaMapShader = 0;
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
	m_Camera = new CameraClass(0.0f, 0.0f, -5.0f);
	if (!m_Camera)
		return false;

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
		return false;

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "square.txt", L"stone01.dds", L"dirt01.dds", L"alpha01.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_AlphaMapShader = new AlphaMapShaderClass;
	if (!m_AlphaMapShader)
		return false;

	// Initialize the color shader object.
	result = m_AlphaMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the alpha map shader object.", "Error", MB_OK);
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
	// Release the color shader object.
	if (m_AlphaMapShader)
	{
		m_AlphaMapShader->Shutdown();
		delete m_AlphaMapShader;
		m_AlphaMapShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
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
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model on the render pipeline.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the scene using the shader.
	result = m_AlphaMapShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), 
									worldMatrix, viewMatrix, projectionMatrix,
									m_Model->GetTextureArray());
	if (!result)
		return false;

	// Present the rendered scene.
	m_D3D->EndScene();

	return true;
}
