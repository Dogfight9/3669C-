#ifndef MYGYRO_H
#define MYGYRO_H

#include "MyDefines.h"
#include "WPILib.h"
#include "PIDSource.h"

class MyGyro
{
public:
	static MyGyro * GetInstance();
	void GyroSet(float value);
	float Get();
	int IsResetGyro();
	void ResetGyro(int value);
	void Turn(float value) {turnValue = value;}
	float GetTurn(){return turnValue;}

	
protected:
	MyGyro();
	
private:
	static MyGyro * instance;
	
	static float gyroRefinedValue;
	static int resetGyro;
	static float turnValue;
	
	
	//static SEM_ID m_GyroSemaphore;
		
	Task * m_gyroRefiner;
};
#endif // MYGYRO_H
