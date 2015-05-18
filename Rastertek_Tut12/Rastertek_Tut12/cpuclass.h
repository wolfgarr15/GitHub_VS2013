/////////////////////////////////
// Filename: cpuclass.h
/////////////////////////////////

/////////////////////////////////
// Pre-processing Directives
//------------------------------
#pragma once

///////////////////////////////
// Linking
//-----------------------------
#pragma comment(lib, "pdh.lib")

///////////////////////////////
// Includes
//-----------------------------
#include <Pdh.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class CPUClass
{
public:
	CPUClass();
	CPUClass(const CPUClass& src);
	~CPUClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCPU;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	ULONG m_lastSampleTime;
	long m_cpuUsage;
};

