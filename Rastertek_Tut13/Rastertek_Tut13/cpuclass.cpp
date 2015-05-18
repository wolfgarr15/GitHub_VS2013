///////////////////////////////////
// Filename: cpuclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "cpuclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
CPUClass::CPUClass() {}

CPUClass::CPUClass(const CPUClass& src) {}

CPUClass::~CPUClass() {}

void CPUClass::Initialize(HWND hwnd)
{
	PDH_STATUS status;

	// Initialize the flag indicating if the object can read the CPU.
	m_canReadCPU = true;

	// Create the query object for polling the CPU.
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		MessageBox(hwnd, "Cannot read the CPU.", "Warning.", MB_OK);
		m_canReadCPU = false;
	}

	// Set query object to poll all cpus in the system.
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCPU = false;
	}

	m_lastSampleTime = GetTickCount();

	m_cpuUsage = 0;

	return;
}

void CPUClass::Shutdown()
{
	if (m_canReadCPU)
		PdhCloseQuery(m_queryHandle);

	return;
}

void CPUClass::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCPU)
	{
		if ((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();
			PdhCollectQueryData(m_queryHandle);
			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);
			m_cpuUsage = value.longValue;
		}
	}

	return;
}

int CPUClass::GetCpuPercentage()
{
	int usage;

	if (m_canReadCPU)
		usage = (int)m_cpuUsage;
	else
		usage = 0;

	return usage;
}
