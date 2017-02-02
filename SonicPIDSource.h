#ifndef SonicPIDSource_H_
#define SonicPIDSource_H_

#include "WPILib.h"
#include "PIDSource.h"
#include "MyDefines.h"
#include "MyUltraSonic.h"

class SonicPIDSource : public PIDSource
{
	
public:
	SonicPIDSource();
	
	double PIDGet();
	
	
private:
	MyUltraSonic * InstSonic;
	
	static double output;
	
};
#endif
