///////////////////////////////////
// Filename: bitmapclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "bitmapclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
BitmapClass::BitmapClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_texture = 0;
}

BitmapClass::BitmapClass(const BitmapClass& src) {}

BitmapClass::~BitmapClass() {}

bool BitmapClass::Initialize(ID3D11Device* device,
							int screenWidth, int screenHeight,
							WCHAR* bitmapFilename,
							int bitmapWidth, int bitmapHeight)
{
	bool result;


	// Store the screen size.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the size in pixels that this bitmap should be rendered.
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// Initialize the previous rendering positions (arbitrary setting).
	m_previousPosX = -1;
	m_previousPosY = -1;

	// Initialize the vertex and pixel buffers.
	result = InitializeBuffers(device);
	if (!result)
		return false;

	// Load the bitmap texture.
	result = LoadTexture(device, bitmapFilename);
	if (!result)
		return false;

	return true;
}

void BitmapClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result;

	// (Re-)build the dynamic vertex buffer for rendering.
	result = UpdateBuffers(deviceContext, positionX, positionY);
	if (!result)
		return false;

	// Put the vertex and index buffers on the render pipeline.
	RenderBuffers(deviceContext);
	
	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;
	VertexType* vertices;
	ULONG* indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	// Crate the index array.
	indices = new ULONG[m_indexCount];
	if (!indices)
		return false;

	// Initialize the vertex array to zeroes to start.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Initialize the index array.
	for (int i = 0; i < m_indexCount; i++)
		indices[i] = i;

	// Setup the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer from the description.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;

	// Setup the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ULONG) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer from the description.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	// Release the vertex and index arrays.
	delete [] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;
}
