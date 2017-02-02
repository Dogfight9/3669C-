#include "SonicPIDOutput.h"

SonicPIDOutput::SonicPIDOutput()
{
	InstMotors = DriveMotors::GetInstance();
}

void SonicPIDOutput::PIDWrite(float output) 
{
	if(InstMotors != NULL) {
		InstMotors->PIDWriteSonic(output);
	}
}

