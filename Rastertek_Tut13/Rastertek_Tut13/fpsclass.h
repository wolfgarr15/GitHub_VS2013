/////////////////////////////////
// Filename: fpsclass.h
/////////////////////////////////

/////////////////////////////////
// Pre-processing Directives
//------------------------------
#pragma once

///////////////////////////////
// Linking
//-----------------------------
#pragma comment(lib, "winmm.lib")

///////////////////////////////
// Includes
//-----------------------------
#include <Windows.h>
#include <mmsystem.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class FPSClass
{
public:
	FPSClass();
	FPSClass(const FPSClass& src);
	~FPSClass();

	void Initialize();
	void Frame();
	int GetFPS();

private:
	int m_FPS;
	int m_count;
	ULONG m_startTime;
};

