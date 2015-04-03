////////////////////////////////
// Filename: d3dclass.cpp
////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "d3dclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}

D3DClass::D3DClass(const D3DClass& src) {}

D3DClass::~D3DClass() {}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	UINT numModes;
	UINT modeWidth;
	UINT modeHeight;
	UINT numerator;
	UINT denominator;
	int error;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView;
	float screenAspect;

	// Store the vsync setting.
	m_vsync_enabled = vsync;
	
	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
		return false;

	// Use the factory to create an adapter for the graphics interface.
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
		return false;

	// Enumerate the primary adapter output.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
		return false;

	// Get the number of display modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM format.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
		return false;

	// Create a list to hold all possible display modes.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return false;

	// Now fill the display mode list.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
		return false;

	// Go through the list of supported display modes and select the one that matches the screen width and height.
	for(UINT i = 0; i < numModes; i++)
	{
		modeWidth = displayModeList[i].Width;
		modeHeight = displayModeList[i].Height;

		if(modeWidth == (UINT)screenWidth && modeHeight == (UINT)screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	// Get the video grapgics adapter (VGA) description.
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
		return false;

	// Store the dedicated VGA memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Store the VGA name.
	error = wcstombs_s(NULL, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
		return false;

	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	SecureZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set the back buffer to a regular 32-bit surface.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
}