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

void BitmapClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left;
	float right;
	float top;
	float bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	// If the position of the bitmap image has not changed, there is nothing to update so return.
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
		return true;

	// If it has changed, update the position.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// Calculate the screen coordinates of the sides of the image. 
	left = (float)positionX - 0.5f * (float)m_screenWidth;
	right = left + (float)m_bitmapWidth;
	bottom = (float)positionY - 0.5f * (float)m_screenHeight;
	top = bottom + (float)m_bitmapHeight;

	// Create a temporary vertex array and fill it with the new values.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	vertices[0].position = D3DXVECTOR3(left, bottom, 0.0f);  // bottom left
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(left, top, 0.0f);     // top left
	vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(right, top, 0.0f);    // top right
	vertices[2].texture = D3DXVECTOR2(1.0f, 0.0f);


	vertices[3].position = D3DXVECTOR3(right, top, 0.0f);    // top right
	vertices[3].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, bottom, 0.0f); // bottom right
	vertices[4].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[5].position = D3DXVECTOR3(left, bottom, 0.0f);  // bottom left
	vertices[5].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	// Get a pointer to the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, sizeof(VertexType) * m_vertexCount);

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the temporary vertex array.
	delete[] vertices;
	vertices = 0;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride;
	UINT offset;

	// Set the vertexx buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer on the render pipeline.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the render topology type.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* texFilename)
{
	bool result;

	// Create the texture object.
	m_texture = new TextureClass;
	if (!m_texture)
		return false;

	// Initialize the texture object.
	result = m_texture->Initialize(device, texFilename);
	if (!result)
		return false;

	return true;
}

void BitmapClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}
