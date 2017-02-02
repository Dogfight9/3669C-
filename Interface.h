#ifndef INTERFACE_H
#define INTERFACE_H

#include "MyDefines.h"
#include "WPILib.h"
#include "DrivePIDOutput.h"
#include "DriveMotors.h"
#include "GyroPIDSource.h"
#include "XboxController.h"
#include "CompressorClass.h"
#include "TheClaw.h"
#include "Camera.h"
#include "MyUltraSonic.h"
#include "OmniJoystick.h"


/*
 *  1. Shooter Motors
 *  2. Solenoid shifting
 *  3. Claw solenoid
 *  4. Cylindrical wall
 *  5. tilt-of-claw motors
 */




class Interface
{
public:
	Interface();
	void SetState(int mode) {state = mode;}
	void SetLeftDriveMotorSpeed(float speed);
	void SetRightDriveMotorSpeed(float speed);
	void ComputeDriveMotorSpeeds(float moveValue, float rotateValue);
	void GoStraight(bool str);
	void Shift(bool on);
	bool GetGear();
	void SetAutonomous(bool on);
	void Shoot();
	void Shift2(bool on);
	void Pickup();
	void Pass();
	string GetSide();
	void Turn(float value);
	void Reset();
	void SetReset(int on);
	double GetDistance();
	void Startup();
	void Open();
	void SetClawOutput(float output);
	float GetClawOutput();
	void Close();
	void SetPivotSpeed(float input);
	void SetClawPosition(int pos);
	int GetClawPosition();
	void Test(bool on);
	void Test2(bool on);
	void ShootSetup();
	void Catch();
	void Shuffle();
	void ShiftLatch();
	void Unfill();
	void ZeroEncoder();
	bool GetReset();
	void SetZero(bool on);
	void ShortPass();
	void Stop();
	void PistonShoot();
	void AutoShoot();
	void LowShoot();
	void SetAuton(bool on);
	bool GetCompressor();
	void SetCompressor(bool on);
	
	
	int GetState() {return state;}
	
	
	 DriveMotors * MyDriveMotors;
	 MyGyro * Gyro;
	 CompressorClass * MyCompr;
	 TheClaw * MyClaw;
	 Camera * TheCamera;
	 MyUltraSonic * Sonic;
	
private:
	//Data values
	static int state;
	static int resetGyro;
	static int position;
	
	
	
	//Singletons
	
	//Tasks
	Task * m_Display;
	Task * m_Autonomous;
	Task * m_Teleop;
	Task * m_Test;
	Task * m_Disabled;
	
};
#endif // Interface_H
