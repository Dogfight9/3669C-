#ifndef OMNI_JOYSTICK_H_
#define OMNI_JOYSTICK_H_

#include "WPILib.h"
#include "TrigCalcs.h"

class OmniJoystick : public Joystick
{
public:
	//construcor
	OmniJoystick(UINT32 port)
		:Joystick(port)
	{
	}
	
	float GetAngle(void);
	float GetR(void);
};

#endif
