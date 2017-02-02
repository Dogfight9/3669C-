#include "GyroPIDSource.h"


GyroPIDSource::GyroPIDSource()
{
	
	InstGyro = MyGyro::GetInstance();
}


double GyroPIDSource::PIDGet() //Name required by PID class
{
	if(InstGyro != NULL){
		return InstGyro->Get();
	} else {
		return 0.0;
	}
}

