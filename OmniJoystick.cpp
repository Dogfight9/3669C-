#include "OmniJoystick.h"

float OmniJoystick::GetAngle(void)
{
	return angleCalc(-GetY(), GetX());
}

float OmniJoystick::GetR(void)
{
	return cartToPolar(GetX(), -GetY());
}
