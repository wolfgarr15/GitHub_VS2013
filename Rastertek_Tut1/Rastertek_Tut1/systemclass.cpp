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
	MSG msg;
	bool done;
	bool result;

	// Initialize the message structure.
	SecureZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If Windows signals to quit, end the application.
		if(msg.message == WM_QUIT)
			done = true;
		else
		{
			// Otherwise perform frame processing.
			result = Frame();
			if(!result)
				done = true;
		}
	}
}

void SystemClass::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
}

bool SystemClass::Frame()
{
	bool result;

	// Check if the user has pressed the escape key.
	if(m_Input->IsKeyDown(VK_ESCAPE))
		return false;

	// Do the frame processing for graphics objects.
	result = m_Graphics->Frame();
	if(!result)
		return false;

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
		// Check if a key has been pressed down.
		case WM_KEYDOWN:
			{
				m_Input->KeyDown((UINT)wparam);
				return 0;
			}

		// Check if a key has been released.
		case WM_KEYUP:
			{
				m_Input->KeyUp((UINT)wparam);
				return 0;
			}

		// Send all other input to the Windows message handler.
		default:
			return DefWindowProc(hwnd, message, wparam, lparam);
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX WndClass;
	DEVMODE dmScreenSettings;
	int posX;
	int posY;

	// Get the application handle.
	g_AppHandle = this;

	// Get the application instance.
	m_hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_AppName = "Engine";

	// Setup the window's default class settings.
	WndClass.cbClsExtra = 0;
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	WndClass.hIconSm = WndClass.hIcon;
	WndClass.hInstance = m_hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = m_AppName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	
	// Register the window class.
	RegisterClassEx(&WndClass);

	// Determine the client's desktop resolution.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it's fullscreen or windowed.
	if(FULL_SCREEN)
	{
		// If fullscreen, set the screen to maximum size and 32-bit color.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (ULONG)screenWidth;
		dmScreenSettings.dmPelsHeight = (ULONG)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL; // CHECK THIS !!!!!!!!!!!!!!!!!!!
	}
}
