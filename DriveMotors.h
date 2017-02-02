#ifndef DriveMotors_H
#define DriveMotors_H

#include "WPILib.h"
#include "PIDOutput.h"
#include "MyDefines.h"
#include "Math.h"

class DriveMotors : public PIDOutput
{
public:
	static DriveMotors * GetInstance();
	
	float  Get();
	float  GetSonic();
	virtual void PIDWrite(float output);
	virtual void PIDWriteSonic(float output);
	void SetLeftSpeed(float output);
	float GetLeftSpeed() {return LeftSpeed;}
	void SetRightSpeed(float output);
	float GetRightSpeed() {return RightSpeed;}
	Talon * GetRightTalon() {return RightMotor;}
	Talon * GetLeftTalon() {return LeftMotor;}
	bool GoingStraight() {return straight;}
	void GoStraight(bool str);
	void Shift(bool on);
	bool GetGear() {return gear;}
	bool IsAutonomous() {return auton;}
	void SetAutonomous(bool on) { auton = on;}

protected:
	DriveMotors();
	
private:
	static DriveMotors * instance;
	static Solenoid * Shifter;
	static Solenoid * Shifter2;
		
	static Talon * RightMotor;
	static Talon * LeftMotor;
	
	static bool straight;
	static bool gear;
	static bool auton;
	static float MOutput;
	static float LeftSpeed;
	static float RightSpeed;
	static float SOutput;
	
	Task * m_MotorTask;
};
#endif
