////////////////////////////////
// Filename: textclass.cpp
////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "textclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
TextClass::TextClass()
{
	m_font = 0;
	m_fontShader = 0;
	m_sentences = 0;
}

TextClass::TextClass(const TextClass& src) {}

TextClass::~TextClass() {}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd,
							int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix, 
							int numSentences)
{
	bool result;

	// Store the screen width and height, and the base view matrix.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_font = new FontClass;
	if (!m_font)
		return false;

	// Initialize the font object.
	result = m_font->Initialize(device, "fontdata.txt", L"font.dds");
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the font object.", "Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_fontShader = new FontShaderClass;
	if (!m_fontShader)
		return false;

	// Initialize the font shader class.
	result = m_fontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, "Could not initialize the font shader object.", "Error", MB_OK);
		return false;
	}

	// Create the sentence objects.
	m_numSentences = numSentences;

	m_sentences = new SentenceType*[m_numSentences];
	if (!m_sentences)
		return false;

	for (int i = 0; i < m_numSentences; i++)
	{
		m_sentences[i] = new SentenceType;
		if (!m_sentences[i])
			return false;
	}

	// Initialize the sentences.
	for (int i = 0; i < m_numSentences; i++)
	{
		result = InitializeSentence(&m_sentences[i], 100, device);
		if (!result)
		{
			MessageBox(hwnd, "Could not initialize a sentence object.", "Error", MB_OK);
			return false;
		}
	}

	return true;
}

void TextClass::Shutdown()
{
	// Release the sentences.
	if (m_sentences)
	{
		for (int i = 0; i < m_numSentences; i++)
			ReleaseSentence(&m_sentences[i]);
		delete [] m_sentences;
		m_sentences = 0;
	}

	// Release the font shader.
	if (m_fontShader)
	{
		m_fontShader->Shutdown();
		delete m_fontShader;
		m_fontShader = 0;
	}

	//  Release the font object.
	if (m_font)
	{
		m_font->Shutdown();
		delete m_font;
		m_font = 0;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	// Draw the sentences.
	for (int i = 0; i < m_numSentences; i++)
	{
		result = RenderSentence(deviceContext, m_sentences[i], worldMatrix, orthoMatrix);
		if (!result)
			return false;
	}
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	ULONG* indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!(*sentence))
		return false;

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * (*sentence)->maxLength;

	// Set the number of indices.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
		return false;

	// Create the index array.
	indices = new ULONG[(*sentence)->indexCount];
	if (!indices)
		return false;

	// Initialize the vertex array to zero.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for (int i = 0; i < (*sentence)->indexCount; i++)
		indices[i] = i;

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
		return false;

	// Setup the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(ULONG) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
		return false;

	// Release the vertex and index arrays.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text,
								int positionX, int positionY,
								float red, float green, float blue,
								ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX;
	float drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for buffer overflow.
	if (numLetters > sentence->maxLength)
		return false;

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
		return false;

	// Initialize the vertex array to zeroes.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	//  Calculate the draw positions.
	drawX = (float)positionX - (float)m_screenWidth / 2.0f;
	drawY = (float)m_screenHeight / 2.0f - (float)positionY;

	// Build the vertex array with the FontClass.
	m_font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	// Get a pointer to the vertex data.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array.
	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	UINT stride;
	UINT offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set the vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer...
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the topolofy type.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_fontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, 
									m_font->GetTexture(), pixelColor);
	if (!result)
		return false;

	return true;
}

bool TextClass::SetFPS(int FPScount, ID3D11DeviceContext* deviceContext)
{
	char tmpString[16];
	char FPSstring[16];
	float red;
	float green;
	float blue;
	bool result;

	// Truncate the FPS to below 10,000.
	if (FPScount > 9999)
		FPScount = 9999;

	// Convert the FPS integer to string format.
	_itoa_s(FPScount, tmpString, 10);

	// Setup the FPS counter string.
	strcpy_s(FPSstring, "FPS : ");
	strcat_s(FPSstring, tmpString);

	// If the FPS is 60 or above, color the text green.
	if (FPScount >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If the FPS is less than 60, color the text yellow.
	if (FPScount < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If the FPS is less than 30, color the text red.
	if (FPScount < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string info.
	result = UpdateSentence(m_sentences[0], FPSstring, 20, 20, red, green, blue, deviceContext);
	if (!result)
		return false;

	return true;
}

bool TextClass::SetCPU(int CPUusage, ID3D11DeviceContext* deviceContext)
{
	char tmpString[16];
	char CPUstring[16];
	bool result;

	// Convert the CPU integer to string format.
	_itoa_s(CPUusage, tmpString, 10);

	// Setup the FPS counter string.
	strcpy_s(CPUstring, "CPU : ");
	strcat_s(CPUstring, tmpString);
	strcat_s(CPUstring, "%");

	// Update the sentence vertex buffer with the new string info.
	result = UpdateSentence(m_sentences[1], CPUstring, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
		return false;

	return true;
}

bool TextClass::SetRenderCount(int renderCount, ID3D11DeviceContext* deviceContext)
{
	char tmpString[32];
	char renderCountString[32];
	bool result;

	// Convert the render count integer to string format.
	_itoa_s(renderCount, tmpString, 10);

	// Setup the counter string.
	strcpy_s(renderCountString, "Render Count : ");
	strcat_s(renderCountString, tmpString);

	// Update the sentence vertex buffer with the new string info.
	result = UpdateSentence(m_sentences[2], renderCountString, 20, 60, 0.0f, 0.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	return true;
}