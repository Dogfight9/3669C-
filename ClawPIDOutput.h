#ifndef ClawPIDOutput_H
#define ClawPIDOutput_H

#include "TheClaw.h"
#include "WPILib.h"
#include "PIDOutput.h"
#include "MyDefines.h"

class ClawPIDOutput : public PIDOutput
{
public:
	ClawPIDOutput();
	virtual void PIDWrite(float output);
	
private:
	TheClaw * Claws;
};
#endif
