#include "Interface.h"

//static SEM_ID m_InterfaceSem;

int Interface::state = -1;    //Initialized value for state so it doesn't do anything (-1)
int Interface::resetGyro = 0;
int Interface::position = 0;



void Display(Interface*ctl) //Was planned to be the only place to display on the dashboard but...
{
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	
	bool gear = false;
	string hot;
	while(1)
	{
		gear = ctl->GetGear();
		hot = ctl->GetSide();
		
		
		if(gear) dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "1");
		else dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "0");
		
		dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "%s       ", hot.c_str());
		dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "%6.5f ", ctl->GetDistance());
		dsLCD->UpdateLCD();
		Wait(0.05);
	}
}

void Autonomous(Interface * ctl) //Scrapped Autonomous code. Moved to MyRobot for access to PID
{
	int phase = -1;
	int counter = 0;
	
	while(1)
	{
		if(ctl->GetState() == AUTONOMOUS)
		{
			ctl->SetAutonomous(true);
			ctl->GoStraight(true);
			if(phase == -1) {
			
			ctl->SetReset(1);
			phase = 0;
			}
			else if(phase == 0)
			{
				ctl->Turn(90);
			} else if(phase == 1)
			{
				printf("phase 0\n");
				ctl->ComputeDriveMotorSpeeds(0.5,0.0);
				counter++;
				if(counter >= 1000)
				{
					phase = 2;
					counter = 0;
					ctl->ComputeDriveMotorSpeeds(0.0,0.0);
					ctl->SetReset(0);
				}
			}
		} else {ctl->GoStraight(false); counter = 0; phase = 0;}
		Wait(0.005);
	}
}

void Teleop(Interface * ctl)  //Main input system
{
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	XboxController * contr = XboxController::GetInstance();
	//OmniJoystick * joy = new OmniJoystick(1);
	
	float TILT = 0.0;
	float X = 0.0;
	float Y = 0.0;
	float Dir = 0.0;
	float clawO = 0.5;
	
	while(1)
	{
		ctl->SetCompressor(ctl->GetCompressor()); //Decide if I want the Compressor on or not
		/*ctl->Reset();
		if(ctl->GetState() == TELEOP)
		{
			ctl->SetAutonomous(false);*/
		
		// Get the Controller inputs
			X = contr->GetX();
			Y = contr->GetY();
			Dir = contr->GetDirection();
			
			/*if(fabs(X) < 0.075) X = 0.0;
			if(fabs(Y) < 0.075) Y = 0.0;*/
			
			if(fabs(Y) > 0.1  || fabs(X) > 0.1) // Scrapped go straight code. Caused gittering when started
			{
				/*if(Dir < 15.0 && Dir > -15.0 && Y > 0.1)
				{
					ctl->GoStraight(true);
					ctl->SetReset(1);
				} else {
					ctl->GoStraight(false);
					ctl->SetReset(0);
				}*/
			
			    ctl->ComputeDriveMotorSpeeds(Y,X);
			} else ctl->ComputeDriveMotorSpeeds(0.0,0.0);
			
		if(contr->GetButton(1, LEFT_BUMPER))
		{
			ctl->Shift(true);
		} else if(contr->GetButton(1, RIGHT_BUMPER)) ctl->Shift(false);
					
		TILT = contr->GetAxis(2,2);
		
		if(fabs(TILT) > 0.1)
		{
			if(ctl->GetClawPosition() != 0)
			{
				ctl->SetClawPosition(0);
				Wait(0.05);
			}
			ctl->SetPivotSpeed(TILT);
			//ctl->SetZero(false);
		} else {
			ctl->SetPivotSpeed(0.0);
		}
		
		clawO = ((contr->GetAxis(2,3) + 1.0)/2.0);
		
		if(fabs(clawO) < 0.1) clawO = 0.0; 
		ctl->SetClawOutput(clawO);
		
		if(contr->GetButton(2, 1))
		{
			ctl->Shuffle();
		}	
		
		if(contr->GetButton(2, 2))
		{
			ctl->ShootSetup();
		}
		
		if(contr->GetButton(2, 4))
		{
			ctl->Pickup();
		} 
		if(contr->GetButton(2, 3))
		{
			ctl->Catch();
		}
		
		if(contr->GetButton(2, 5))
		{
			ctl->Stop();
		}
		
		if(contr->GetButton(2, 6))
		{
			ctl->SetClawPosition(1);
		} 
		
		if(contr->GetButton(2, 7))
		{
			ctl->SetClawPosition(2);
		}
		
		if(contr->GetButton(2, 10))
		{
			ctl->ShiftLatch();
		}
		
		if(contr->GetButton(2, 11))
		{
			ctl->Unfill();
		}
		
		if(contr->GetButton(2, 8))
		{
			ctl->SetClawPosition(3);
		}
		
		if(contr->GetButton(2,9))
		{
			ctl->SetClawPosition(4);
		}
		
		if(contr->GetButton(1, 1))
		{
			ctl->ShortPass();
		} 
		if(contr->GetButton(1,2))
		{
			ctl->Shoot();
		}
		
		if(contr->GetButton(1,8))
		{
			ctl->Close();
		}
		
		if(contr->GetButton(1, 7))
		{
			ctl->Stop();
		}
		
		if(contr->GetButton(1, 4))
		{
			ctl->PistonShoot();
		}
		
		if(contr->GetButton(1, 3))
		{
			ctl->AutoShoot();
		}
		
		if(contr->GetButton(1, 10))
		{
			ctl->LowShoot();
		}
		
		
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "ClawO: %f", clawO);
		//dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "A: %5.2f  R: %5.2f ", joy->GetAngle(), joy->GetR());
		//dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "Y: %f", Y);
		dsLCD->UpdateLCD();
		
		
		Wait(0.005);
			
			
		
		
		
	}
}

/*void Test(Interface * ctl)
{
	while(1)
	{
		if(ctl->GetState() == TEST)
		{
			ctl->SetAutonomous(false);
		}
		Wait(0.005);
	}
}*/

void Disabled(Interface * ctl)
{
	while(1)
	{
		if(ctl->GetState() == DISABLED)
		{
			ctl->SetAutonomous(false);
			ctl->SetLeftDriveMotorSpeed(0.0);
			ctl->SetRightDriveMotorSpeed(0.0);
		}
		Wait(0.005);
	}
}

Interface::Interface()

{
	//m_InterfaceSem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
	
	//Instances
	MyDriveMotors = DriveMotors::GetInstance();
	Gyro = MyGyro::GetInstance();
	MyCompr = CompressorClass::GetInstance();
	MyClaw = TheClaw::GetInstance();
	//TheCamera = Camera::GetInstance();
	Sonic = MyUltraSonic::GetInstance();
	
	
	//Tasks
	m_Display = new Task("3669Display", (FUNCPTR)Display, 100);
	//m_Display->Start();
	m_Autonomous = new Task("3669Auto", (FUNCPTR)Autonomous, 100);
	//m_Autonomous->Start();
	m_Teleop = new Task("3669Teleop", (FUNCPTR)Teleop, 100);
	m_Teleop->Start();
	//m_Test = new Task("3669Test", (FUNCPTR)Test, 100);
	//m_Test->Start();
    m_Disabled = new Task("3669Disabled", (FUNCPTR)Disabled, 100);
    //m_Disabled->Start();
    
    //Robot Start up
}

void Interface::SetLeftDriveMotorSpeed(float speed)
{
	MyDriveMotors->SetLeftSpeed(speed);
}

void Interface::SetRightDriveMotorSpeed(float speed)
{
	MyDriveMotors->SetRightSpeed(speed);
}

void Interface::ComputeDriveMotorSpeeds(float moveValue, float rotateValue)
{
	float leftMotorOutput;
	float rightMotorOutput;
				  
	//moveValue = moveValue * moveValue * moveValue;
	rotateValue = (rotateValue * rotateValue * rotateValue) * 0.75;
	
	if(moveValue >= 0){
		moveValue = moveValue * moveValue;
	} else {
		moveValue = -1*(moveValue * moveValue);
	}
	/*if(rotateValue >= 0) {
		rotateValue = rotateValue * rotateValue * rotateValue * rotateValue;
	} else {
		rotateValue = -1*(rotateValue * rotateValue * rotateValue * rotateValue);
	}*/
	
	if (moveValue > 0.0)
	{
		if (rotateValue > 0.0)
		{
			leftMotorOutput = moveValue - rotateValue;
			rightMotorOutput = max(moveValue, rotateValue);
		}
		else
		{
			leftMotorOutput = max(moveValue, -rotateValue);
			rightMotorOutput = moveValue + rotateValue;
		}
	}
	else
	{
		if (rotateValue > 0.0)
		{
			leftMotorOutput = - max(-moveValue, rotateValue);
			rightMotorOutput = moveValue + rotateValue;
		}
		else
		{
		    leftMotorOutput = moveValue - rotateValue;
			rightMotorOutput = - max(-moveValue, -rotateValue);
		}
	}
	SetLeftDriveMotorSpeed(leftMotorOutput);
	SetRightDriveMotorSpeed(rightMotorOutput);
}

void Interface::GoStraight(bool str)
{
	MyDriveMotors->GoStraight(str);
}

void Interface::Shift(bool on)
{
	MyDriveMotors->Shift(on);
}

bool Interface::GetGear()
{
	if(MyDriveMotors != NULL)
	{
		return MyDriveMotors->GetGear();
	}
	return true;
}

void Interface::SetAutonomous(bool on)
{
	//if(MyDriveMotors != NULL)
	//{
		MyDriveMotors->SetAutonomous(on);
	//}
}

void Interface::Shoot()
{
	//if(MyClaw != NULL)
	//{
		MyClaw->Shoot();
	//}
}

void Interface::Shift2(bool on)
{
		//MyClaw->Shift(on);
}

string Interface::GetSide()
{
	string result = "unknown";//TheCamera->hotSide();
	
	return result;
}

void Interface::Pickup()
{
	MyClaw->Pickup();
}

void Interface::Pass()
{
	MyClaw->Pass();
}

void Interface::Turn(float value)
{
	Gyro->Turn(value);
}

void Interface::Reset()
{
	//semTake(m_InterfaceSem, WAIT_FOREVER);
	int temp = resetGyro;
	//semGive(m_InterfaceSem);
	Gyro->ResetGyro(temp);
}

void Interface::SetReset(int on)
{
	//semTake(m_InterfaceSem, WAIT_FOREVER);
	resetGyro = on;
	//semGive(m_InterfaceSem);
}

double Interface::GetDistance()
{
	double range = 0.0;
	range = Sonic->GetIN();
	return range;
}

void Interface::Startup()
{
	MyClaw->Startup();
}

void Interface::Open()
{
	MyClaw->Open();
}

void Interface::SetClawOutput(float output)
{
	MyClaw->SetOutput(output);
}

float Interface::GetClawOutput()
{
	return MyClaw->GetOutput();
}

void Interface::Close()
{
	MyClaw->Close();
}

void Interface::SetPivotSpeed(float input)
{
	MyClaw->SetPivotSpeed(input);
}

void Interface::SetClawPosition(int pos)
{
	position = pos;
}

int Interface::GetClawPosition()
{
	return position;
}

void Interface::Test(bool on)
{
	MyClaw->Test(on);
}

void Interface::Test2(bool on)
{
	MyClaw->Test2(on);
}

void Interface::ShootSetup()
{
	MyClaw->ShootSetup();
}

void Interface::Catch()
{
	MyClaw->Catch();
}

void Interface::Shuffle()
{
	MyClaw->Shuffle();
}

void Interface::ShiftLatch()
{
	MyClaw->CloseLatch();
}

void Interface::Unfill()
{
	MyClaw->Unfill();
}

void Interface::ZeroEncoder()
{
	MyClaw->ZeroEncoder();
}

bool Interface::GetReset()
{
	return MyClaw->GetReset();
}

void Interface::SetZero(bool on)
{
	MyClaw->SetReset(on);
}

void Interface::ShortPass()
{
	MyClaw->ShortPass();
}

void Interface::Stop()
{
	MyClaw->Stop();
}

void Interface::PistonShoot()
{
	MyClaw->PistonShoot();
}

void Interface::AutoShoot()
{
	MyClaw->AutomatedShoot();
}

void Interface::LowShoot() 
{
	MyClaw->LowShoot();
}

void Interface::SetAuton(bool on)
{
	Sonic->SetAuton(on);
}

bool Interface::GetCompressor()
{
	return MyClaw->GetCompressor();
}

void Interface::SetCompressor(bool on)
{
	MyCompr->SetCompr(on);
}









