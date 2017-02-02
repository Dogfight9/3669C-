#include "MyUltraSonic.h"

static SEM_ID m_SonicSemaphore;

MyUltraSonic * MyUltraSonic::instance = NULL;

double MyUltraSonic::sonicValue = 0.0;
bool MyUltraSonic::auton = false;

void SonicReader(MyUltraSonic * ctl)
{
	int i = 0;
	int total = 0;
	float sum = 0.0;
	float tempValue = 0.0;
	float value[10] = {};
	float finalValue = 0.0;
	float prevValue = 0.0;
	bool aut = false;
	AnalogChannel * SonicChannel = new AnalogChannel((UINT32)2);
	AnalogUltrasonic * MySonicSensor = new AnalogUltrasonic(SonicChannel, true, 0.0588, 2.4892, 6.0, 254.0);
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	/*
	 *   /|\ These values need to change base off of Sensor
	 *    |
	 *    |
	 *    |
	 */

	while(1)
	{
		aut = ctl->GetAuton();
		tempValue = MySonicSensor->GetRangeInInches();
		//(SonicChannel->GetVoltage()/((DriverStation::GetInstance()->GetBatteryVoltage()/2.4)/10240.0)) / 25.4;
		/*if(value[10] != 0.0) 
		{
			if(tempValue > 30.0 + prevValue || tempValue < prevValue - 30.0)
			{
				tempValue = prevValue;
			}
		}*/
		if(aut)
		{
			if(tempValue > 50.0 + prevValue || tempValue < prevValue - 50.0)
			{
				tempValue = prevValue;
			}
		}
		value[i] = tempValue;
		
		if(i < 9)
		{
			i++;
		} else {
			i = 0;
		}
		
		for(int j = 0; j < 10; j++)
		{
			if(value[j] != 0.0)
			{
				sum += value[j];
				total++;
			}
		}
		finalValue = sum/((float)total);
		total = 0;
		finalValue = finalValue + 0.5;
		
		dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "Sonic: %6.0f \"", finalValue);
		dsLCD->UpdateLCD();
		ctl->SetValue(finalValue);
		prevValue = finalValue;
		sum = 0.0;
		Wait(0.01);
	}
}

MyUltraSonic::MyUltraSonic()
{
	m_SonicSemaphore = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
	
	
	m_SonicTask = new Task("3669Sonic", (FUNCPTR)SonicReader, 100);
	m_SonicTask->Start();
}

MyUltraSonic * MyUltraSonic::GetInstance()
{
	if(instance == NULL){
		instance = new MyUltraSonic();
	}
	return instance;
}

double MyUltraSonic::GetIN()
{
	double output = 0.0;
	semTake(m_SonicSemaphore, WAIT_FOREVER);
	output = sonicValue;
	semGive(m_SonicSemaphore);
	return output;
}

void MyUltraSonic::SetValue(double value)
{
	semTake(m_SonicSemaphore, WAIT_FOREVER);
	sonicValue = value;
	semGive(m_SonicSemaphore);
}

bool MyUltraSonic::GetAuton()
{
	bool output;
	semTake(m_SonicSemaphore, WAIT_FOREVER);
	output = auton;
	semGive(m_SonicSemaphore);
	return output;
}

void MyUltraSonic::SetAuton(bool on)
{
	semTake(m_SonicSemaphore, WAIT_FOREVER);
	auton = on;
	semGive(m_SonicSemaphore);
}












