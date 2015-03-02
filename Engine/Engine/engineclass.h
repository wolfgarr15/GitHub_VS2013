////////////////////////////////////
// Filename: engineclass.h
////////////////////////////////////

////////////////////////////////////
// Pre-processing Directives
//---------------------------------
#pragma once
#define WIN32_LEAN_AND_MEAN

////////////////////////////////////
// Includes
//----------------------------------
#include <Windows.h>

//////////////////////////////////////
// Class Declaration
//------------------------------------
class EngineClass
{
public:
	EngineClass();
	EngineClass(const EngineClass& src);
	~EngineClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
};

