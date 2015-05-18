///////////////////////////////////
// Filename: fpsclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "fpsclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
FPSClass::FPSClass() {}

FPSClass::FPSClass(const FPSClass& src) {}

FPSClass::~FPSClass() {}

void FPSClass::Initialize()
{
	m_FPS = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void FPSClass::Frame()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_FPS = m_count;
		m_count = 0;
		m_startTime = timeGetTime();
	}

	return;
}

int FPSClass::GetFPS()
{
	return m_FPS;
}
