#include "DrivePIDOutput.h"

DrivePIDOutput::DrivePIDOutput()
{
	InstMotors = DriveMotors::GetInstance();
}

void DrivePIDOutput::PIDWrite(float output) 
{
	if(InstMotors != NULL) {
		InstMotors->PIDWrite(output);
	}
}

