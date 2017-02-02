#include "DriveMotors.h"
static SEM_ID m_MotorSemaphore;

float  DriveMotors::MOutput = 0.0;
float  DriveMotors::SOutput = 0.0;
float  DriveMotors::LeftSpeed = 0.0;
float  DriveMotors::RightSpeed = 0.0;
bool   DriveMotors::straight = false;
bool   DriveMotors::gear = true;
bool   DriveMotors::auton = false;

Talon * DriveMotors::RightMotor = NULL;   // Supposed to be Talons
Talon * DriveMotors::LeftMotor = NULL;

DriveMotors * DriveMotors::instance = NULL;
Solenoid * DriveMotors::Shifter = NULL;
Solenoid * DriveMotors::Shifter2 = NULL;

void MotorTask(DriveMotors * ctl) {
	RobotDrive * MyDrive = new RobotDrive(ctl->GetLeftTalon(), ctl->GetRightTalon());
	
	float rightSpeed = 0.0;
	float leftSpeed = 0.0;
	
	// Adjusts because the motors move better in one direction than another
	float rightAdjust = 0.24;
	float leftAdjust = 0.2;
	
	MyDrive->SetMaxOutput(1.0);
	MyDrive->SetExpiration(0.1);
	
	while(1) 
		{
		
			//Get the speeds set for the motors
			rightSpeed = ctl->GetRightSpeed();
			leftSpeed = ctl->GetLeftSpeed();
			
			
			
			if(ctl->GoingStraight()){  //If you want to drive straight (almost always on)
				if(ctl->Get() > 0.01 || ctl->Get() <-0.01)
				{
					rightSpeed = rightSpeed + ctl->Get();
					leftSpeed = leftSpeed - ctl->Get();
				}
				
				if(ctl->IsAutonomous()) //If you want to use the ultrasonic sensor
				{
					rightSpeed = rightSpeed - ctl->GetSonic();
					leftSpeed = leftSpeed - ctl->GetSonic();
				}
			}
			
			//Add in the adjustments
			if(rightSpeed < 0) {
				MyDrive->SetLeftRightMotorOutputs(leftSpeed ,rightSpeed + (rightSpeed * rightAdjust));}
			else if (rightSpeed >= 0){
				MyDrive->SetLeftRightMotorOutputs(leftSpeed + (leftSpeed * leftAdjust),rightSpeed); }
							
			Wait(0.005);
		}
}

DriveMotors::DriveMotors()
{
    m_MotorSemaphore = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
    
    RightMotor = new Talon(RIGHT_DRIVE_MOTOR);
    LeftMotor = new Talon(LEFT_DRIVE_MOTOR);
    
    //To avoid null pointer errors
    if(RightMotor != NULL) {
    	RightMotor->Set(0.0);
    	RightMotor->EnableDeadbandElimination(true);
    }
    
    if(LeftMotor != NULL) {
    	LeftMotor->Set(0.0);
    	LeftMotor->EnableDeadbandElimination(true) ;
    }
    
    Shifter = new Solenoid((UINT8)1,(UINT32)1);
    Shifter->Set(true);
    
    
    m_MotorTask = new Task("3669Motors", (FUNCPTR)MotorTask, 100);
    m_MotorTask->Start();
}

DriveMotors * DriveMotors::GetInstance()
{
	if(instance == NULL){
		instance = new DriveMotors();
	}
	return instance;
}

float DriveMotors::Get() 
{
	semTake(m_MotorSemaphore, WAIT_FOREVER);
	float output = MOutput;
	semGive(m_MotorSemaphore);
	return output;
}

float DriveMotors::GetSonic()
{
	semTake(m_MotorSemaphore, WAIT_FOREVER);
	float output = SOutput;
	semGive(m_MotorSemaphore);
	return output;
}

void DriveMotors::PIDWrite(float output) 
{
	semTake(m_MotorSemaphore, WAIT_FOREVER);
	MOutput = output;
	semGive(m_MotorSemaphore);
	
}

void DriveMotors::SetLeftSpeed(float output) 
{
	semTake(m_MotorSemaphore, WAIT_FOREVER);
	LeftSpeed = output;
	semGive(m_MotorSemaphore);
	
}

void DriveMotors::SetRightSpeed(float output) 
{
	semTake(m_MotorSemaphore, WAIT_FOREVER);
	RightSpeed = output;
	semGive(m_MotorSemaphore);
	
}

void DriveMotors::GoStraight(bool str) 
{
	semTake(m_MotorSemaphore, WAIT_FOREVER);
	straight = str;
	semGive(m_MotorSemaphore);
}

void DriveMotors::Shift(bool on)
{
	if(Shifter != NULL)
	{
		gear = on;
		Shifter->Set(on);
	}
}

void DriveMotors::PIDWriteSonic(float output)
{
	semTake(m_MotorSemaphore, WAIT_FOREVER);
	SOutput = output;
	semGive(m_MotorSemaphore);
}







