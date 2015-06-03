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
	m_Model1 = 0;
	m_Model2 = 0;
	m_TextureShader = 0;
	m_TransparentShader = 0;
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
	m_Model1 = new ModelClass;
	if (!m_Model1)
		return false;

	// Initialize the Model1 object.
	result = m_Model1->Initialize(m_D3D->GetDevice(), "Cube.txt", L"dirt01.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the Model1 object.", "Error", MB_OK);
		return false;
	}

	// Create the Model2 object.
	m_Model2 = new ModelClass;

	// Initialize the Model2 object.
	result = m_Model2->Initialize(m_D3D->GetDevice(), "Cube.txt", L"stone01.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the Model2 object.", "Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
		return false;

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return false;
	}

	// Create the transparent shader object.
	m_TransparentShader = new TransparentShaderClass;
	if (!m_TransparentShader)
		return false;

	// Initialize the transparent shader.
	result = m_TransparentShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the transparent shader object.", "Error", MB_OK);
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
	// Release the transparent shader object.
	if (m_TransparentShader)
	{
		m_TransparentShader->Shutdown();
		delete m_TransparentShader;
		m_TextureShader = 0;
	}

	// Release the color shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model2 object.
	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the model1 object.
	if (m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = 0;
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
	static float rotation = 0.0f;
	float blendAmount = 0.5f;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX Rx, Ry, Rz;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Apply the rotation transformation.
	D3DXMatrixRotationX(&Rx, rotation);
	D3DXMatrixRotationY(&Ry, rotation);
	D3DXMatrixRotationZ(&Rz, rotation);
	//worldMatrix = worldMatrix * Rx * Ry * Rz; // DISABLE ROTATIONS FOR NOW!!!

	// Put the model on the render pipeline.
	m_Model1->Render(m_D3D->GetDeviceContext());

	// Render the scene using the shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model1->GetIndexCount(), 
									worldMatrix, viewMatrix, projectionMatrix,
									m_Model1->GetTexture());
	if (!result)
		return false;

	// Translate to the right by one half unit and towards the camera by one unit.
	D3DXMatrixTranslation(&worldMatrix, 0.5f, 0.0f, -1.0f);

	// Turn on alpha blending for the transparency to work.
	m_D3D->TurnOnAlphaBlending();

	// Put the second square model on the graphics pipeline.
	m_Model2->Render(m_D3D->GetDeviceContext());

	// Render the second square model with the stone texture and use the 50% blending amount for transparency.
	result = m_TransparentShader->Render(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_Model2->GetTexture(), blendAmount);
	if (!result)
		return false;

	// Translate to the left by one half unit.
	D3DXMatrixTranslation(&worldMatrix, 0.5f, 0.0f, 0.0f);

	// Turn off alpha blending.
	m_D3D->TurnOffAlphaBlending();

	// Present the rendered scene.
	m_D3D->EndScene();

	// Update the rotation.
	rotation += 0.005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;

	return true;
}
