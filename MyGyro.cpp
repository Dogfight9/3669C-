#include "MyGyro.h"

MyGyro * MyGyro::instance = NULL;

float  MyGyro::gyroRefinedValue = 0.0;
int MyGyro::resetGyro = 0;
float MyGyro::turnValue = 0;

DriverStationLCD * dsLCDGyro = DriverStationLCD::GetInstance();

static void GyroRefiner(MyGyro * ctl) 
{
	
	Gyro  MyActualGyro((UINT32)GYRO);
	MyActualGyro.Reset();
	MyActualGyro.SetSensitivity(.00667);
	
	float currentGyroValue = 0.0;
	int currReset = 0;
	float drift = 0.0;   // drift per 5ms wait period; +
	float totalDrift = 0.0;
	float turn = 0.0;
	float difference = 0.0;
	float prev = 0.0;
	while(1)
	{
		turn = ctl->GetTurn();
		if(ctl->IsResetGyro() == 1 && currReset == 0) {
			MyActualGyro.Reset();
			totalDrift = 0.0;
			currReset = 1;
			printf("ResetGyro \n");
		} else if(ctl->IsResetGyro() == 0) currReset = 0;
		currentGyroValue = MyActualGyro.GetAngle();
		dsLCDGyro->Printf(DriverStationLCD::kUser_Line5, 1, "%6.2f", currentGyroValue);
		dsLCDGyro->UpdateLCD();
		
		if(!(turn > -0.5 && turn < 0.5))
		{
			currentGyroValue = currentGyroValue + turn;
			turn = turn - difference;
		} else {turn = 0.0;}
		
		currentGyroValue = currentGyroValue - totalDrift;
	
		if(currentGyroValue > 180.0) {
			ctl->GyroSet(currentGyroValue - 360.0);
		} else if (currentGyroValue < -180.0) {
			ctl->GyroSet(currentGyroValue + 360.0);
		} else {
			ctl->GyroSet(currentGyroValue);
		}
		
		if(!(difference >= -0.15 && difference <= 0.15))
		{
			difference = currentGyroValue - prev;
		}
		
		prev = currentGyroValue;
		
		
		totalDrift = totalDrift + drift;
		Wait(0.005);
	}
}

MyGyro::MyGyro()
{
	//m_GyroSemaphore = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
	    
	m_gyroRefiner = new Task("3669Gyro", (FUNCPTR)GyroRefiner, 100);
	m_gyroRefiner->Start();
    Wait(1.0);
}

MyGyro * MyGyro::GetInstance()
{
	if(instance == NULL){
		instance = new MyGyro();
	}
	return instance;
}

void MyGyro::GyroSet(float value)
{
	gyroRefinedValue = value;
	dsLCDGyro->Printf(DriverStationLCD::kUser_Line5, 1, "%6.2f", gyroRefinedValue);
	dsLCDGyro->UpdateLCD();
}

float MyGyro::Get() //Name required by PID class
{
	float test = gyroRefinedValue;
	return test;
}

void MyGyro::ResetGyro(int value){
	resetGyro = value;
}

int MyGyro::IsResetGyro() {
	int output = resetGyro;
	return output;
}

