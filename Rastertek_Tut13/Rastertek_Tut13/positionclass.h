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
	void GetRotation(float& rotationX, float& rotationY);

	void TurnUp(bool keyDown);
	void TurnDown(bool keyDown);
	void TurnLeft(bool keyDown);
	void TurnRight(bool keyDown);

private:
	float m_frameTime;
	float m_rotationX;
	float m_rotationY;
	float m_upTurnSpeed;
	float m_downTurnSpeed;
	float m_leftTurnSpeed;
	float m_rightTurnSpeed;
};

