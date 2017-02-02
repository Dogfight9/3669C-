#include "ClawPIDOutput.h"

ClawPIDOutput::ClawPIDOutput()
{
	Claws = TheClaw::GetInstance();
}

void ClawPIDOutput::PIDWrite(float output) 
{
	if(Claws != NULL) {
		Claws->PIDWrite(output);
	}
}

