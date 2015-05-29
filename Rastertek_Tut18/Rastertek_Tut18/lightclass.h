////////////////////////////////
// Filename: lightclass.h
////////////////////////////////

////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <D3DX10math.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& src);
	~LightClass();

	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetSpecularColor(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);
	void SetSpecularPower(float power);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR4 GetSpecularColor();
	D3DXVECTOR3 GetDirection();
	float GetSpecularPower();

private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR4 m_specularColor;
	D3DXVECTOR3 m_direction;
	float m_specularPower;
};

