////////////////////////////////
// Filename: textureshaderclass.cpp
////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "textureshaderclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
TextureShaderClass::TextureShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_samplerState = 0;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& src) {}

TextureShaderClass::~TextureShaderClass() {}

bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the shaders.
	result = InitializeShader(device, hwnd, L"vTexture.hlsli", L"pTexture.hlsli");
	if (!result)
		return false;

	return true;
}

void TextureShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders.
	ShutdownShader();
	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
								D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
								ID3D11ShaderResourceView* texture)
{
	bool result;

	// Set the shader parameters.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
		return false;

	// Render with the set parameters.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	UINT numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize pointers.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile th vertex shader code.
	result = D3DX11CompileFromFileW(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
									&vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile, it should have written an error message.
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);

		// Otherwise, the program must have failed to find the shader file.
		else
			MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFileW(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
									&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile, it should have written an error message.
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);

		// Otherwise, the program must have failed to find the shader file.
		else
			MessageBoxW(hwnd, psFilename, L"Missing Shader File", MB_OK);
	}

	// Create the vertex shader.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
		return false;

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
		return false;

	// HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
