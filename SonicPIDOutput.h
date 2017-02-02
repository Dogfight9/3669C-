#ifndef SonicPIDOutput_H
#define SonicPIDOutput_H

#include "DriveMotors.h"
#include "WPILib.h"
#include "PIDOutput.h"
#include "MyDefines.h"

class SonicPIDOutput : public PIDOutput
{
public:
	SonicPIDOutput();
	virtual void PIDWrite(float output);
	
private:
	DriveMotors * InstMotors;
};
#endif
