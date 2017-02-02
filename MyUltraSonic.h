#ifndef MyUltraSonic_H
#define MyUltraSonic_H

#include "MyDefines.h"
#include "WPILib.h"
#include "analogultrasonic.h"

class MyUltraSonic
{
public:
	static MyUltraSonic * GetInstance();
	double GetIN();
	void SetValue(double value);
	bool GetAuton();
	void SetAuton(bool on);

protected:
	MyUltraSonic();
	
private:
	static MyUltraSonic * instance;
	//static SEM_ID m_SonicSemaphore;
	
	static double sonicValue;
	static bool auton;
	
	
	Task * m_SonicTask;
	
};
#endif // MyUltraSonic_H
