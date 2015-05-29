////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////

////////////////////////////////
// Class Header
//------------------------------
#include "lightclass.h"

///////////////////////////////
// Class Definition
//-----------------------------
LightClass::LightClass() {}

LightClass::LightClass(const LightClass& src) {}

LightClass::~LightClass() {}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}