////////////////////////////////
// Filename: inputclass.h
////////////////////////////////

////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once
#define DIRECTINPUT_VERSION_0x0800

////////////////////////////////
// Linking
//------------------------------
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

///////////////////////////////
// Includes
//-----------------------------
#include <dinput.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass& src);
	~InputClass();

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& mouseX, int& mouseY);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	UCHAR m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	int m_screenWidth;
	int m_screenHeight;
	int m_mouseX;
	int m_mouseY;
};

