#include "XboxController.h"

XboxController * XboxController::instance = NULL;
Joystick * XboxController::joystick1 = NULL;
Joystick * XboxController::joystick2 = NULL;

DriverStationLCD * dsLCDController = DriverStationLCD::GetInstance();

XboxController::XboxController()
{
	joystick1 = new Joystick((UINT32)1);
	joystick2 = new Joystick((UINT32)2);
}

XboxController * XboxController::GetInstance()
{
	if(instance == NULL){
		instance = new XboxController();
	}
	return instance;
}

float XboxController::GetAxis(int joystick, int axis)
{
	float output = 0.0;
	if(joystick == 1)
	{
		if(joystick1 != NULL)
		{
			output = joystick1->GetRawAxis((UINT32)axis);
		}
	}
	else if(joystick == 2)
	{
		if(joystick2 != NULL)
		{
			output = joystick2->GetRawAxis((UINT32)axis);
		}
	}
	return output;
}

float XboxController::GetX()
{
	float output = 0.0;
	if(joystick1 != NULL) {
		output = (1*(joystick1->GetX()));
	}
	
	if (output <= 0.1 && output >= -0.1)
	{
		output = 0.0;
	} else if (output < -0.1)
	{
		output = (output + 0.1) / 0.9;
	} else if (output > 0.1)
	{
		output = (output - 0.1) / 0.9;
	}
	return output;
}

float XboxController::GetY()
{
	float output = 0.0;
	if(joystick1 != NULL) {
		output = (-1*(joystick1->GetY()));
	}
	
	if (output <= 0.1 && output >= -0.1)
	{
		output = 0.0;
	} else if (output < -0.1)
	{
		output = (output + 0.1) / 0.9;
	} else if (output > 0.1)
	{
		output = (output - 0.1) / 0.9;
	}
	return output;
}

float XboxController::GetDirection()
{
	double x = (double)GetX();
	double y = (double)GetY();
	double result = 0.0;
	
	if(x > -0.075 && x < 0.075)
	{
		if(y > -0.05)
		{
			result = 0.0;
		} else {
			result = 180.0;
		}
	} else if (y >= 0){
		result = 90 - (atan(fabs(y)/fabs(x))*180/3.14159265);
		if(x < 0) result = -(result);
	} else if (y < 0) {
		result = 90 + (atan(fabs(y)/fabs(x))*180/3.14159265);
		if(x < 0) result = -(result);
	}
		
	return (float)result;
}

bool XboxController::GetButton(int joystick, int button)
{
	bool pressed = false;
	if(joystick == 1)
	{
		if(joystick1 != NULL)
		{
			pressed = joystick1->GetRawButton((UINT32)button);
		}
	}
	else if(joystick == 2)
	{
		if(joystick2 != NULL)
		{
			pressed = joystick2->GetRawButton((UINT32)button);
		}
	}
	return pressed;
}

Joystick * XboxController::GetJoystick() 
{
	return joystick1;
}


