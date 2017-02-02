#ifndef DrivePIDOutput_H
#define DrivePIDOutput_H

#include "DriveMotors.h"
#include "WPILib.h"
#include "PIDOutput.h"
#include "MyDefines.h"

class DrivePIDOutput : public PIDOutput
{
public:
	DrivePIDOutput();
	virtual void PIDWrite(float output);
	
private:
	DriveMotors * InstMotors;
};
#endif
