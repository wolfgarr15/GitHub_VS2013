////////////////////////////////
// Filename: positionclass.h
////////////////////////////////

////////////////////////////////
// Pre-processing Directives
//-----------------------------
#pragma once

///////////////////////////////
// Includes
//-----------------------------
#include <math.h>

///////////////////////////////
// Class Declaration
//-----------------------------
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass& src);
	~PositionClass();

	void SetFrameTime(float time);
	void GetRotation(float& rotationY);

	void TurnLeft(bool keyDown);
	void TurnRight(bool keyDown);

private:
	float m_frameTime;
	float m_rotationY;
	float m_leftTurnSpeed;
	float m_rightTurnSpeed;
};

