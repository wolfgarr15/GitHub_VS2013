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
		MessageBox(m_hWnd, "Failed to create input object.", "Error!", MB_OK);
		return false;
	}

	// Initialize the input object.
	m_Input->Initialize();
	if(!m_Input)
	{
		MessageBox(m_hWnd, "Failed to initialize input object.", "Error!", MB_OK);
		return false;
	}

	// Create the graphics object.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		MessageBox(m_hWnd, "Failed to create graphics object.", "Error!", MB_OK);
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hWnd);
	if(!result)
	{
		MessageBox(m_hWnd, "Failed to initialize graphics object.", "Error!", MB_OK);
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
	DWORD dwStyle;
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
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;                       // CHECK THIS !!!!!!!!!!!!!!!!!!!

		// Change the display settings to fullscreen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		
		// Set the window display style.
		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

		// Set the postion of the window to the top left corner.
		posX = 0;
		posY = 0;
	}
	else
	{
		// If windowed, set to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Set the window display style.
		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window.
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_AppName, m_AppName, dwStyle,
							posX, posY, screenWidth, screenHeight, 
							NULL, NULL, m_hInstance, NULL);

	// Show the window and set it as the main focus.
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Fix the display settings if the app was fullscreen.
	if(FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	// Remove the window.
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_AppName, m_hInstance);
	m_hInstance = 0;

	// Release the application handle.
	g_AppHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
			return g_AppHandle->MessageHandler(hwnd, message, wparam, lparam);
	}
}
