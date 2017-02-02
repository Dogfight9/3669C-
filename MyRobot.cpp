#include "MyDefines.h"
#include "Interface.h"
#include "SonicPIDSource.h"
#include "SonicPIDOutput.h"
#include "ClawPIDOutput.h"

DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();

class RobotDemo : public IterativeRobot
{
	Interface       TheInterface;
	GyroPIDSource   MyGyroPIDSource;
	DrivePIDOutput  MyMotorPIDOutput;
	PIDController   GyroPID;
	SonicPIDOutput  MySonicPIDOutput;
	SonicPIDSource  MySonicPIDSource;
	PIDController   SonicPID;
	ClawPIDOutput   MyClawPIDOutput;
	Encoder         ClawEncoder;
	PIDController   TiltPID;
	int             phase;
	int             counter;
	
public:
	RobotDemo():
		TheInterface     (),
		MyGyroPIDSource  (),
		MyMotorPIDOutput (),
		GyroPID          (0.012, 0.0, 0.075, &MyGyroPIDSource, &MyMotorPIDOutput, 0.05),
		MySonicPIDOutput (),
		MySonicPIDSource (),
		SonicPID         (0.013, 0.00000004, 0.0000000000000001, &MySonicPIDSource, &MySonicPIDOutput, 0.05),
		MyClawPIDOutput  (),
		ClawEncoder      (6, 7, false, k4X),
		TiltPID          (0.019, 0.0,   0.0, &ClawEncoder, &MyClawPIDOutput, 0.05), //  
		phase            (-1),
		counter          (0)
	{
		this->SetPeriod(0); 	//Set update period to sync with robot control packets (20ms nominal)
	}
	

void RobotDemo::RobotInit() {
	TheInterface.SetState(ROBOT_INIT);
	GyroPID.Disable();
	GyroPID.SetContinuous(false);
	GyroPID.SetOutputRange(-0.75,0.75);
	SonicPID.Disable();
	SonicPID.SetContinuous(false);
	SonicPID.SetOutputRange(-0.75,0.75);
	TiltPID.Disable();
	TiltPID.SetContinuous(false);
	TiltPID.SetOutputRange(-1.0, 1.0);
	ClawEncoder.Start();                    //Start encoder now so that any change made to the arm after bootup would be registered
}

void RobotDemo::DisabledInit() {
	TheInterface.SetState(DISABLED);
	GyroPID.Disable();
	SonicPID.Disable();
	TiltPID.Disable();
}

void RobotDemo::DisabledPeriodic() {
}

void RobotDemo::AutonomousInit() {
	GyroPID.Enable();
	GyroPID.SetTolerance(.0025);
	GyroPID.SetSetpoint(0.0);
	SonicPID.Disable();
	SonicPID.SetTolerance(.025);
	TiltPID.Enable();
	TiltPID.SetTolerance(10.0);
	TiltPID.SetSetpoint(0.0);
	TheInterface.SetAuton(true);
}

void RobotDemo::AutonomousPeriodic() {
	//TODO Move autonomous here
	TheInterface.SetAutonomous(true);
	TheInterface.GoStraight(true);
	string side = TheInterface.GetSide();
	
	//-355 Tilt for Autonmous scoring
	
	dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "Encoder: %d       ", ClawEncoder.Get());
	dsLCD->UpdateLCD();

	if(phase == -1) //Beginning of Autonomous
	{
		TheInterface.SetReset(1);
		TheInterface.Startup();
		TheInterface.Test2(true);
		Wait(0.75);
		phase = 1;
	} /*else if(phase == 5)                         Preliminary Camera Code
	{
		TiltPID.SetSetpoint(-1100);
		if(fabs(TiltPID.GetError()) < 10.0)
		{
			counter++;
			if(counter > 7) {
				phase = 0;
				counter = 0;
			}
		} else counter = 0;
	} else if(phase == 0)
		
	{
		if(side.compare("Unknown") == 0)
		{
		
		} else if(side.compare("Left") == 0)
		{
			phase = -2;
		} else if(side.compare("Right") == 0)
		{
			phase = -2;
		}
	}*/ else if(phase == 1)    //Move Forward set distance in inches
	{
		SonicPID.Enable();
		SonicPID.SetSetpoint(100.0);
		if(fabs(SonicPID.GetError()) < 10.0)
		{
			counter++;
			if(counter > 5){
				phase = 2;
				SonicPID.Disable();
				counter = 0;
				TheInterface.ShootSetup();
				//GyroPID.Disable();
			}
		} else counter = 0;
	} else if(phase == 2)      // Rotate Claw a certain amount of encoder counts
	{
		TiltPID.SetSetpoint(160.0);
		
		if(fabs(TiltPID.GetError()) < 10){
			counter++;
			if(counter > 10){
				phase = 3;
				counter = 0;
				
			}
		} else counter = 0;
	} else if(phase == 3)     // Fire Cannon!
	{
		//Wait(0.5);
		TheInterface.Shoot();
		Wait(1.0);
		
		phase = 4;
	} else if(phase == 4)    // Reset claw position
	{
		TiltPID.SetSetpoint(0.0);
		phase = 5;
	}
}

void RobotDemo::TeleopInit() {
	TheInterface.SetState(TELEOP);
	GyroPID.Disable();
	GyroPID.SetTolerance(.025);
	GyroPID.SetSetpoint(0.0);
	SonicPID.Disable();
	TiltPID.Disable();
	TiltPID.SetTolerance(0.025);
	TiltPID.SetSetpoint(0.0);
	TheInterface.SetAuton(false);
}

void RobotDemo::TeleopPeriodic() {
	int position = TheInterface.GetClawPosition();
	if(position == 0) {
		TiltPID.Disable();
	} else if (position != 0) 
	{
		TiltPID.Enable();
		if(position == 1)                  //Different preset positions for certain spots on the field
		{
			TiltPID.SetSetpoint(55.0);
		} else if(position == 2) {
			TiltPID.SetSetpoint(160.0);
		} else if(position == 3) {
			TiltPID.SetSetpoint(0.0);
		} else if(position == 4) {
			TiltPID.SetSetpoint(100.0);
		}
	}
	dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "Encoder: %d       ", ((ClawEncoder.Get()))); // For degrees, / by 5.68
	dsLCD->UpdateLCD();
}

void RobotDemo::TestInit() {
	//TheInterface.SetState(TEST);
}

void RobotDemo::TestPeriodic() {
}

};

START_ROBOT_CLASS(RobotDemo);

