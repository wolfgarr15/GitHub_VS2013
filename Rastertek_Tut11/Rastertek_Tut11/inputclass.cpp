////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "inputclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
InputClass::InputClass() 
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

InputClass::InputClass(const InputClass& src) {}

InputClass::~InputClass() {}

bool InputClass::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	// Store the screen dimensions.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the mouse location in the app window.
	m_mousePosition = { 0, 0 };

	// Initialize the direct input interface.
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
		return false;

	// Initialize the interface to the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
		return false;

	// Set the data format for the keyboard input.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return false;

	// Acquire the keyboard to begin receiving input.
	result = m_keyboard->Acquire();
	if (FAILED(result))
		return false;

	// Initialize the interface to the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
		return false;

	// Set the data format for the mouse input.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	// Set the cooperative level of the mouse to be non-exclusive. NOTE: Must check for when the app goes out of focus!
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
		return false;

	return true;
}

void InputClass::Shutdown()
{
	// Unacquire the mouse and release the interface.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Unaqcuire the keyboard and release the interface.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the direct input interface.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;

	// Read the keyboard.
	result = ReadKeyboard();
	if (!result)
		return false;

	// Read the mouse.
	result = ReadMouse();
	if (!result)
		return false;

	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (void*)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus, try to reacquire it.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	// Read the mouse.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus, try to reacquire it.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_mouse->Acquire();
		else
			return false;
	}

	return true;
}

void InputClass::ProcessInput()
{
	// Update the mouse location.
	m_mousePosition.x += m_mouseState.lX;
	m_mousePosition.y += m_mouseState.lY;

	// Ensure the mouse does not go off screen.
	if (m_mousePosition.x < 0) { m_mousePosition.x = 0; }
	if (m_mousePosition.y < 0) { m_mousePosition.y = 0; }
	if (m_mousePosition.x > m_screenWidth) { m_mousePosition.x = m_screenWidth; }
	if (m_mousePosition.y > m_screenHeight) { m_mousePosition.y = m_screenHeight; }

	return;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise "and" on the keyboard state to check if the escape key is pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
		return true;
	
	return false;
}

void InputClass::GetMouseLocation(POINT& mousePosition)
{
	mousePosition = m_mousePosition;
	return;
}