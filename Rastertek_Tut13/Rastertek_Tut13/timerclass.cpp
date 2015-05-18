///////////////////////////////////
// Filename: timerclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "timerclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
TimerClass::TimerClass() {}

TimerClass::TimerClass(const TimerClass& src) {}

TimerClass::~TimerClass() {}

bool TimerClass::Initialize()
{
	// Check to see if the system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
		return false;

	// Find out how many times the frequency counter ticks per millisecond.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;

	return;
}

float TimerClass::GetTime()
{
	return m_frameTime;
}
