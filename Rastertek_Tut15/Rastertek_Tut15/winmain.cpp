//////////////////////////////////
// Filename: winmain.cpp
//////////////////////////////////

///////////////////////////////////
// Includes
//----------------------------------
#include "systemclass.h"

///////////////////////////////////
// Windows Entry Point
//--------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdShow)
{
	SystemClass* System = 0;
	bool result = false;

	// Create the system object.
	System = new SystemClass;
	if(!System)
		return 1;

	// Initialize the system.
	result = System->Initialize();
	if(!result)
	{
		System->Shutdown();
		delete System;
		System = 0;
		return 2;
	}

	// Run the System.
	System->Run();

	// Shutdown the system.
	System->Shutdown();
	delete System;
	System = 0;
	return 0;
}

