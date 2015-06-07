/////////////////////////////////
// Filename: colorshaderclass.h
/////////////////////////////////

/////////////////////////////////
// Pre-processing Directives
//------------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <D3DX10math.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class CameraClass
{
public:
	CameraClass();
	CameraClass(float x, float y, float z);
	CameraClass(const CameraClass& src);
	~CameraClass();

	void SetPosition(float x, float y, float z);
	void SetRotation(float rotX, float rotY, float rotZ);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX& viewMatrix);

private:
	float m_positionX;
	float m_positionY;
	float m_positionZ;
	float m_rotationX;
	float m_rotationY;
	float m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

