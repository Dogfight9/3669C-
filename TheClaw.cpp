#include "TheClaw.h"

TheClaw * TheClaw::instance = NULL;

float TheClaw::reqOutput = 0.5;
float TheClaw::clawSpeed = 0.0;
float TheClaw::pivotSpeed = 0.0;
float TheClaw::pivotSpeedPID = 0.0;
bool TheClaw::longArms = false;
bool TheClaw::shortArms = false;
bool TheClaw::latch = false;
bool TheClaw::retract = false;
bool TheClaw::pancak = false;
bool TheClaw::shooterFill = false;
bool TheClaw::shooterDump = false;
bool TheClaw::open = true;
bool TheClaw::pos = false;
bool TheClaw::shot = false;
bool TheClaw::passed = false;
bool TheClaw::shuffle = false;
bool TheClaw::zero = false;
bool TheClaw::reset = false;
bool TheClaw::stop = false;
bool TheClaw::shoot = false;
bool TheClaw::pistonS = false;
bool TheClaw::autoS = false;
bool TheClaw::shoot2 = false;
bool TheClaw::compressor = false;
float TheClaw::topSpin = false;

static SEM_ID m_ClawSemaphore;


/*
 * 
 *  4 Pickup
 *  5 Stop motors, close arms
 *  1 (trigger) shoot
 *  3 Pass
 *  2 Catch
 *  Y axis up-down movement of claw
 *  11 & 10 increase-decrease power pistons/motors
 */

void ClawTask(TheClaw * ctl)
{
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	
	Talon * Claw1 = new Talon((UINT32)3);
	Talon * Claw2 = new Talon((UINT32)4);
	Talon * Claw3 = new Talon((UINT32)5);
	Talon * Turn  = new Talon((UINT32)6);
	Solenoid * LongArms = new Solenoid((UINT8)1, (UINT32)2);
	Solenoid * ShortArms = new Solenoid ((UINT8)1, (UINT32)3);
	Solenoid * Latch = new Solenoid ((UINT8)1, (UINT32)4);
	Solenoid * Pancake = new Solenoid((UINT8)1, (UINT32)6);
	Solenoid * ShooterFill = new Solenoid ((UINT8)1, (UINT32)7);
	Solenoid * ShooterDump = new Solenoid ((UINT8)1, (UINT32)8);
	
	//Encoder * Encoder1 = new Encoder(8,9);
	
	//PIDController * RPM = new PIDController(0.0,0.0,0.0,Encoder1, Claw1, 0.05);
	
	DigitalInput * Catch = new DigitalInput((UINT32)3);
	DigitalInput * FrontStop = new DigitalInput((UINT32)4);
	DigitalInput * BackStop = new DigitalInput((UINT32)5);
	DigitalInput * limit = new DigitalInput((UINT32)2);
	
	int counter = 0;
	int counter2 = 0;
	int counter3 = 0;
	int counter4 = 0;
	int counter5 = 0;
	int counter6 = 0;
	int counter7 = 0;
	int counter8 = 0;
	int counter9 = 0;
	int counter10 = 0;
	float modifier = 1.0;
	float clawS = 0.0;
	float pivotS = 0.0;
	bool longA = false;
	bool shortA = false;
	bool panc = false;
	bool shooterF = false;
	bool shooterD = false;
	bool lat = true;
	bool op = true;
	bool sh = false;
	bool pass = false;
	bool shuff = false;
	bool zer = false;
	bool sto = false;
	bool shoo = false;
	bool piston = false;
	bool autonS = false;
	bool sh2 = false;
	float topS = 0.0;
	
	bool position = false;
	
	while(1)
	{
		
		
		position = ctl->GetPosition();
		modifier = ctl->GetOutput();
		clawS = ctl->GetClawS() * modifier;
		op = ctl->IfOpen();
		sh = ctl->IfShot();
		pass = ctl->IfPassed();
		shuff = ctl->GetShuffle();
		zer = ctl->GetZero();
		sto = ctl->GetStop();
		shoo = ctl->GetShoot();
		piston = ctl->GetPistonS();
		autonS = ctl->GetAutoS();
		topS = ctl->GetTopSpin();
		sh2 = ctl->GetShoot2();
		
		if(!(position))
		{
			pivotS = ctl->GetPivotS();
		} else {
			pivotS = ctl->GetPivotSPID();
		} 
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "P: %d B: %d F: %d", limit->Get(), BackStop->Get(), FrontStop->Get());
		dsLCD->UpdateLCD();
		
		if(FrontStop->Get() == 0 && pivotS < 0.0)
		{
			pivotS = 0.0;
		} else if(BackStop->Get() == 0 && pivotS > 0.0) {
			pivotS = 0.0;
		}
		
		longA = ctl->GetLongArms();
		shortA = ctl->GetShortArms();
		lat = ctl->GetLatch();
		panc = ctl->GetPancake();
		shooterF = ctl->GetShooterFill();
		shooterD = ctl->GetShooterDump();
		
			if(Catch->Get() == 0 && op)
			{
				counter9++;
				longA = false;
				ctl->OpenLong();
				panc = false;
				ctl->SetPancake(false);
				if(counter9 > 20)
				{
					clawS = 0.0;
					ctl->SetClawSpeed(0.0);
					shortA = false;
					ctl->OpenShort();
					op = false;
					ctl->Closed();
					counter9 = 0;
				}
			}
			
			if(sh)
			{
				counter++;
				if(counter > 200 && counter < 250)
				{
					shooterF = false;
					ctl->SetShooterFill(false);
					shooterD = true;
					ctl->SetShooterDump(true);
					clawS = 0.0;
					ctl->SetClawSpeed(0.0);
					ctl->SetCompressor(false);
					counter = 250;
				}
				else if(limit->Get() == 0 && counter >= 250)
				{
					if(counter > 300) {
						sh = false;
						ctl->SetShot(false);
						shooterD = false;
						ctl->SetShooterDump(false);
						lat = false;
						ctl->ShiftLatch(false);
						longA = false;
						ctl->OpenLong();
						shortA = false;
						ctl->OpenShort();
						panc = false;
						ctl->SetPancake(false);
						counter = -1;
					}
				} 
			} else if(counter < 0)
			{
				counter--;
				if(counter < -200) {
					shooterF = true;
					ctl->SetShooterFill(true);
					counter = 0;
				}
			}
			
			if(sh2)
			{
				counter10++;
				if(counter10 > 10 && counter10 < 12)
				{
					panc = false;
					ctl->SetPancake(false);
					counter10 = 12;
				}
				else if(counter10 > 32)
				{
					shooterF = false;
					ctl->SetShooterFill(false);
					lat = true;
					ctl->ShiftLatch(true);
					sh = true;
					ctl->SetShot(true);
					counter10 = 0;
					sh2 = false;
					ctl->SetShoot2(false);
				}
			}
			
			if(pass)
			{
				counter2++;
				if(counter2 > 200 && counter2 < 250)
				{
					/*clawS = 0.0;
					ctl->SetClawSpeed(0.0);*/
					panc = false;
					ctl->SetPancake(false);
					counter2 = 250;
				} else if(counter2 > 350)
				{
					clawS = 0.0;
					ctl->SetClawSpeed(0.0);
					shortA = false;
					ctl->OpenShort();
					pass = false;
					ctl->SetPassed(false);
					counter2 = 0;
				}
			}
			
			if(shuff)
			{
				counter3++;
				if(counter3 > 50 && counter3 < 60)
				{
					/*clawS = -1.0;
					ctl->SetClawSpeed(-1.0);*/
					panc = false;
					ctl->SetPancake(false);
					counter3 = 60;
				} else if(counter3 > 160 && counter3 < 170)
				{
					/*clawS = 0.0;
					ctl->SetClawSpeed(0.0);*/
					shortA = false;
					ctl->OpenShort();
					counter3 = 0;
					shuff = false;
					ctl->SetShuffle(false);
				}
			}
			
			if (zer)
			{
				counter4++;
				if(counter4 > 400)
				{
					lat = true;
					ctl->ShiftLatch(true);
					counter4 = 0;
					zer = false;
					ctl->SetZero(false);
				}
			}
			
			if(shoo)
			{
				counter5++;
				if(counter5 > 20)
				{
					clawS = 1.0;
					ctl->SetClawSpeed(1.0);
					shoo = false;
					ctl->SetShoot(false);
					
					counter5 = 0;
				}
			}
			
			if(piston)
			{
				counter7++;
				if(counter7 > 20)
				{
					shooterF = false;
					ctl->SetShooterFill(false);
					lat = true;
					ctl->ShiftLatch(true);
					sh = true;
					ctl->SetShot(true);
					counter7 = 0;
					piston = false;
					ctl->SetPistonS(false);
				}
			}
			
			if(autonS)
			{
				counter8++;
				if(counter8 > 170)
				{
					ctl->Shoot();
					counter8 = 0;
					autonS = false;
					ctl->SetAutoS(false);
				}
			}
			
			if(sto)
			{
				counter6++;
				if(counter6 > 30)
				{
					longA = false;
					ctl->OpenLong();
					shortA = false;
					ctl->OpenShort();
					panc = false;
					ctl->SetPancake(false);
					sto = false;
					ctl->SetStop(false);
					ctl->SetCompressor(false);
					counter6 = 0;
				}
			}
			
		
		Claw1->Set(clawS + topS);
		Claw2->Set(clawS);
		Claw3->Set(-1.0*(clawS));
		Turn->SetSpeed(pivotS);
		LongArms->Set(longA);
		ShortArms->Set(shortA);
		Latch->Set(lat);
		Pancake->Set(panc);
		ShooterFill->Set(shooterF);
		ShooterDump->Set(shooterD);
		
		
		
		Wait(0.005);
	}
}

TheClaw::TheClaw()
{
	m_ClawSemaphore = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
    
	
    m_ClawTask = new Task("3669Claw", (FUNCPTR)ClawTask, 100);
    m_ClawTask->Start();
}

TheClaw * TheClaw::GetInstance()
{
	if(instance == NULL){
		instance = new TheClaw();
	}
	return instance;
}

void TheClaw::Shoot()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shortArms = true;
	shoot2 = true;
	semGive(m_ClawSemaphore);
	
	/*Wait(0.17);
	
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shortArms = true;
	semGive(m_ClawSemaphore);
	
	Wait(0.2);
	
	/*semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterFill = false;
	latch = true;
	shot = true;
	semGive(m_ClawSemaphore);
	
	/*Wait(1.0);
	
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterFill = false;
	shooterDump = true;
	clawSpeed = 0.0;
	shot = true;
	semGive(m_ClawSemaphore);
	/*while(!(limit->Get() == 1))
	{
		Wait(0.005);
	}
	
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterDump = false;
	latch = false;
	semGive(m_ClawSemaphore);
	
	Wait(0.25);
	
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterFill = true;
	longArms = false;
	shortArms = true;
	pancak = false;
	semGive(m_ClawSemaphore);*/
	
}

void TheClaw::OpenLong()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	longArms = false;
	semGive(m_ClawSemaphore);
}

void TheClaw::CloseLong()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	longArms = true;
	semGive(m_ClawSemaphore);
}

void TheClaw::OpenShort()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shortArms = false;
	semGive(m_ClawSemaphore);
}

void TheClaw::CloseShort()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shortArms = true;
	semGive(m_ClawSemaphore);
}



void TheClaw::Pickup()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	open = true;
	shortArms = true;
	pancak = true;
	longArms = false;
	clawSpeed = -1.0;
	semGive(m_ClawSemaphore);
	
}

void TheClaw::Pass()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	longArms = false;
	pancak = false;
	/*semGive(m_ClawSemaphore);
	
	Wait(0.25);
	
	semTake(m_ClawSemaphore, WAIT_FOREVER);*/
	shortArms = true;
	passed = true;
	semGive(m_ClawSemaphore);
}

void TheClaw::ShiftLatch(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	latch = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::Open()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shortArms = true;
	longArms = true;
	//clawSpeed = 0.5;
	open = true;
	semGive(m_ClawSemaphore);
}

float TheClaw::GetClawS()
{
	float speed = 0.0;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	speed = clawSpeed;
	semGive(m_ClawSemaphore);
	return speed;
}

float TheClaw::GetPivotS()
{
	float speed = 0.0;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	speed = pivotSpeed;
	semGive(m_ClawSemaphore);
	return speed;
}

bool TheClaw::GetLongArms()
{
	bool on = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	on = longArms;
	semGive(m_ClawSemaphore);
	return longArms;
}

bool TheClaw::GetShortArms()
{
	bool on = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	on = shortArms;
	semGive(m_ClawSemaphore);
	return on;
}

bool TheClaw::GetLatch()
{
	bool on = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	on = latch;
	semGive(m_ClawSemaphore);
	return on;
}

bool TheClaw::GetRetract()
{
	bool on = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	on = retract;
	semGive(m_ClawSemaphore);
	return on;
}

bool TheClaw::GetShooterFill()
{
	bool on = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	on = shooterFill;
	semGive(m_ClawSemaphore);
	return on;
}

bool TheClaw::GetShooterDump()
{
	bool on = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	on = shooterDump;
	semGive(m_ClawSemaphore);
	return on;
}

void TheClaw::Startup()
{
	Pickup();
}

bool TheClaw::GetPancake()
{
	bool on = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	on = pancak;
	semGive(m_ClawSemaphore);
	return on;
}

bool TheClaw::IfOpen()
{
	bool on = true;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	on = open;
	semGive(m_ClawSemaphore);
	return on;
}

void TheClaw::Closed()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	open = false;
	semGive(m_ClawSemaphore);
}

void TheClaw::SetOutput(float output)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	reqOutput = output;
	semGive(m_ClawSemaphore);
}

float TheClaw::GetOutput()
{
	float output = 0.0;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = reqOutput;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::PIDWrite(float output)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	pivotSpeedPID = output;
	if(output != 0.0) pos = true;
	else pos = false;
	semGive(m_ClawSemaphore);
}

void TheClaw::Close()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	clawSpeed = 0.0;
	topSpin = 0.0;
	compressor = false;
	semGive(m_ClawSemaphore);
}

void TheClaw::SetPivotSpeed(float input)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	pivotSpeed = input;
	semGive(m_ClawSemaphore);
}

/*void TheClaw::SetRPM(float output)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	clawSpeed
	semGive(m_ClawSemaphore);
}*/

void TheClaw::Test(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shortArms = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::Test2(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterFill = on;
	semGive(m_ClawSemaphore);	
}

void TheClaw::ShootSetup()
{
	/*
	 * Pancake
	 * motors on
	 */
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	longArms = false;
	shortArms = false;
	pancak = true;
	shooterFill = true;
	compressor = true;
	shoot = true;
	semGive(m_ClawSemaphore);
}

float TheClaw::GetPivotSPID()
{
	float output = 0.0;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = -1 * pivotSpeedPID;
	semGive(m_ClawSemaphore);
	return output;
}

bool TheClaw::GetPosition()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = pos;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetClawSpeed(float input)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	clawSpeed = input;
	semGive(m_ClawSemaphore);
}


void TheClaw::SetPancake(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	pancak = on;
	semGive(m_ClawSemaphore);
}

bool TheClaw::IfShot()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = shot;
	semGive(m_ClawSemaphore);
	return shot;
}

void TheClaw::SetShot(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shot = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::SetShooterDump(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterDump = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::Catch() 
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	open = true;
	shortArms = true;
	pancak = true;
	longArms = true;
	clawSpeed = -0.875;
	semGive(m_ClawSemaphore);
}

void TheClaw::SetShooterFill(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterFill = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::SetPassed(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	passed = on;
	semGive(m_ClawSemaphore);
}

bool TheClaw::IfPassed()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = passed;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::Shuffle()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shortArms = true;
	pancak = true;
	shuffle = true;
	semGive(m_ClawSemaphore);
}

bool TheClaw::GetShuffle()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = shuffle;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetShuffle(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shuffle = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::Unfill()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterFill = false;
	shooterDump = true;
	zero = true;
	//latch = true;
	semGive(m_ClawSemaphore);
}

void TheClaw::CloseLatch()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shooterDump = false;
	latch = false;
	semGive(m_ClawSemaphore);
}

void TheClaw::ZeroEncoder()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	zero = true;
	semGive(m_ClawSemaphore);
}

void TheClaw::SetZero(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	zero = on;
	semGive(m_ClawSemaphore);
}

bool TheClaw::GetZero()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = zero;
	semGive(m_ClawSemaphore);
	return zero;
}

bool TheClaw::GetReset()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = reset;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetReset(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	reset = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::ShortPass()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	clawSpeed = 1.0;
	passed = true;
	pancak = true;
	shortArms = true;
	semGive(m_ClawSemaphore);
}

void TheClaw::Stop()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	clawSpeed = 0.0;
	topSpin = 0.0;
	stop = true;
	semGive(m_ClawSemaphore);
}

bool TheClaw::GetStop()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = stop;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetStop(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	stop = on;
	semGive(m_ClawSemaphore);
}

bool TheClaw::GetShoot()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = shoot;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetShoot(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shoot = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::PistonShoot()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	longArms = false;
	shortArms = true;
	pancak = true;
	shooterFill = true;
	pistonS = true;
	semGive(m_ClawSemaphore);
}

bool TheClaw::GetPistonS()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = pistonS;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetPistonS(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	pistonS = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::AutomatedShoot()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	ShootSetup();
	autoS = true;
	semGive(m_ClawSemaphore);
}

bool TheClaw::GetAutoS()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = autoS;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetAutoS(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	autoS = on;
	semGive(m_ClawSemaphore);
}

void TheClaw::LowShoot()
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	topSpin = 1.0;
	semGive(m_ClawSemaphore);
}

float TheClaw::GetTopSpin()
{
	float output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = topSpin;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetTopSpin(float on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	topSpin = on;
	semGive(m_ClawSemaphore);
}

bool TheClaw::GetShoot2()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = shoot2;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetShoot2(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	shoot2 = on;
	semGive(m_ClawSemaphore);
}

bool TheClaw::GetCompressor()
{
	bool output = false;
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	output = compressor;
	semGive(m_ClawSemaphore);
	return output;
}

void TheClaw::SetCompressor(bool on)
{
	semTake(m_ClawSemaphore, WAIT_FOREVER);
	compressor = on;
	semGive(m_ClawSemaphore);
}

	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	







