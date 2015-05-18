///////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "fontclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
FontClass::FontClass()
{
	m_font = 0;
	m_texture = 0;
}

FontClass::FontClass(const FontClass& src) {}

FontClass::~FontClass() {}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;

	// Load the font data.
	result = LoadFontData(fontFilename);
	if (!result)
		return false;

	// Load the font texture.
	result = LoadTexture(device, textureFilename);
	if (!result)
		return false;

	return true;
}

void FontClass::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
	return;
}

bool FontClass::LoadFontData(char* fontFilename)
{
	ifstream fin;
	int numChar;
	char tmp;

	// Initialize the number for characters in the font.
	numChar = 95;

	// Freate the font spacing buffer.
	m_font = new FontType[numChar];
	if (!m_font)
		return false;

	// Read the font size and spacing between characters.
	fin.open(fontFilename);
	if (fin.fail())
		return false;

	for (int i = 0; i < numChar; i++)
	{
		fin.get(tmp);
		while (tmp != ' ')
			fin.get(tmp);

		fin.get(tmp);
		while (tmp != ' ')
			fin.get(tmp);

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	// Close the font data file.
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (m_font)
	{
		delete[] m_font;
		m_font = 0;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* textureFilename)
{
	bool result;

	// Create the texture object.
	m_texture = new TextureClass;
	if (!m_texture)
		return false;

	// Load the texture from the resource file.
	result = m_texture->Initialize(device, textureFilename);
	if (!result)
		return false;

	return true;
}

void FontClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters;
	int index;
	int letter;
	float letterHeighth;

	// Coerce the input vertices into a VertexType struct.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index and letter heighth.
	index = 0;
	letterHeighth = 16.0f;

	// Draw each letter ontpo a quad.
	for (int i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space, then just move over three pixels.
		if (letter == 0)
			drawX = drawX + 3.0f;

		else
		{
			// First triangle in the quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f); // Top left
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - letterHeighth), 0.0f); // Bottom right
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - letterHeighth), 0.0f); // Bottom left
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 1.0f);
			index++;

			// Second Triangle in the quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f); // Top left
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - letterHeighth), 0.0f); // Bottom right
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			// Update the location of the drawing by the size of the letter plus one pixel.
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}

	return;
}