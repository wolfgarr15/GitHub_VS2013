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
							int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
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
	m_numSentences = 2;

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
		result = UpdateSentence(m_sentences[i], "Hello", 100, (100 * (i + 1)), 1.0f, 0.0f, 0.0f, deviceContext);
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

// START INITIALIZE SENTENCE !!!
