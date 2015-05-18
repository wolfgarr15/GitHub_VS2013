////////////////////////////////
// Filename: systemclass.h
////////////////////////////////

////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once
#define WIN32_LEAN_AND_MEAN

///////////////////////////////
// Includes
//-----------------------------
#include <Windows.h>
#include "inputclass.h"
#include "graphicsclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"

///////////////////////////////
// Class Declaration
//-----------------------------
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& src);
	~SystemClass();

	bool Initialize();
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

private:
	LPCSTR m_AppName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	FPSClass* m_FPS;
	CPUClass* m_CPU;
	TimerClass* m_Timer;
};

///////////////////////////////
// Global OS Callback function
//-----------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

/////////////////////////////
// Global App Handle
//---------------------------
static SystemClass* g_AppHandle = 0;
