/////////////////////////////////
// Filename: timerclass.h
/////////////////////////////////

/////////////////////////////////
// Pre-processing Directives
//------------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <Windows.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass& src);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

