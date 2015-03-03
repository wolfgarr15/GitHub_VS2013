////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "systemclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& src) {}

SystemClass::~SystemClass() {}

bool SystemClass::Initialize()
{
	int screenWidth = 0;
	int screenHeight = 0;
	bool result = false;

	// Initialize the Windpows API.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.
	m_Input = new InputClass;
	if(!m_Input)
	{
		MessageBox(m_hwnd, "Failed to create input object.", "Error!", MB_OK);
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize();
	if(!result)
	{
		MessageBox(m_hwnd, "Failed to initialize input object.", "Error!", MB_OK);
		return false;
	}

	// Create the graphics object.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		MessageBox(m_hwnd, "Failed to create graphics object.", "Error!", MB_OK);
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		MessageBox(m_hwnd, "Failed to initialize graphics object.", "Error!", MB_OK);
		return false;
	}

	return true;
}

void SystemClass::Run()
{
	// STUFF!!!
}

void SystemClass::Shutdown()
{
	// STUFF!!!
}
