#include "SonicPIDSource.h"

double SonicPIDSource::output = 6000.0;

SonicPIDSource::SonicPIDSource()
{
	
	InstSonic = MyUltraSonic::GetInstance();
}


double SonicPIDSource::PIDGet() //Name required by PID class
{
	if(InstSonic != NULL){
		return InstSonic->GetIN();
	} else {
		return 0.0;
	}
}

