#ifndef GYROPIDSOURCE_H_
#define GYROPIDSOURCE_H_

#include "WPILib.h"
#include "PIDSource.h"
#include "MyDefines.h"
#include "MyGyro.h"

class GyroPIDSource : public PIDSource
{
	
public:
	GyroPIDSource();
	
	double PIDGet();
	
	
private:
	MyGyro * InstGyro;
	
};
#endif
