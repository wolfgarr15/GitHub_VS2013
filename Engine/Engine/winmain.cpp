///////////////////////////////
// Filename: winmain.cpp
///////////////////////////////

///////////////////////////////
// Includes
//-----------------------------
#include "engineclass.h"

///////////////////////////////
// Windows entry
//-----------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdShow)
{
	EngineClass* Engine;
	bool bResult;

	// Create the Engine object.
	Engine = new EngineClass;
	if(!Engine)
		return 1;

	// Initialize and run the engine.
	bResult = Engine->Initialize();
	if(bResult)
		Engine->Run();

	// Shutdown the engine.
	Engine->Shutdown();
	delete Engine;
	Engine = 0;

	return 0;
}