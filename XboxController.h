#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include "WPILib.h"
#include "MyDefines.h"
#include "Math.h"

class XboxController
{
public:
	static XboxController * GetInstance();
	float GetAxis(int joystick, int axis);
	float GetX();
	float GetY();
	float GetDirection();
	bool  GetButton(int joystick, int button);
	Joystick * GetJoystick();
	
protected:
	XboxController();
	
private:
	static XboxController * instance;
	
	static Joystick * joystick1;
	static Joystick * joystick2;
	
};
#endif
