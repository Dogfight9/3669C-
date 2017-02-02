#ifndef TheClaw_H
#define TheClaw_H

#include "WPILib.h"
#include "MyDefines.h"
#include "Math.h"

class TheClaw
{
public:
	static TheClaw * GetInstance();
	void Shoot();
	void OpenLong();
	void CloseLong();
	void OpenShort();
	void CloseShort();
	void Pickup();
	void Pass();
	void ShiftLatch(bool on);
	void Open();
	float GetClawS();
	float GetPivotS(); 
	float GetPivotSPID();
	bool GetLongArms();
	bool GetShortArms();
	bool GetLatch();
	bool GetRetract();
	bool GetPancake();
	bool GetShooterFill();
	bool GetShooterDump();
	bool IfOpen();
	void Startup();
	void Closed();
	void SetOutput(float output);
	float GetOutput();
	void PIDWrite(float output);
	void Close();
	void SetPivotSpeed(float input);
	void Test(bool on);
	void Test2(bool on);
	void ShootSetup();
	bool GetPosition();
	void SetClawSpeed(float input);
	int GetLimit();
	void SetPancake(bool on);
	bool IfShot();
	void SetShot(bool on);
	void SetShooterDump(bool on);
	void Catch();
	void SetShooterFill(bool on);
	void SetPassed(bool on);
	bool IfPassed();
	void Shuffle();
	bool GetShuffle();
	void SetShuffle(bool on);
	void Unfill();
	void CloseLatch();
	void ZeroEncoder();
	void SetZero(bool on);
	bool GetZero();
	bool GetReset();
	void SetReset(bool on);
	void ShortPass();
	void Stop();
	bool GetStop();
	void SetStop(bool on);
	bool GetShoot();
	void SetShoot(bool on);
	void PistonShoot();
	bool GetPistonS();
	void SetPistonS(bool on);
	void AutomatedShoot();
	bool GetAutoS();
	void SetAutoS(bool on);
	void LowShoot();
	void SetTopSpin(float on);
	float GetTopSpin();
	bool GetShoot2();
	void SetShoot2(bool on);
	void SetCompressor(bool on);
	bool GetCompressor();
	
protected:
	TheClaw();
	
private:
	static TheClaw * instance;
	
	static float reqOutput;
	static float clawSpeed;
	static float pivotSpeed;
	static float pivotSpeedPID;
	static bool longArms;
	static bool shortArms;
	static bool latch;
	static bool retract;
	static bool pancak;
	static bool shooterFill;
	static bool shooterDump;
	static bool open;
	static bool pos;
	static bool shot;
	static bool passed;
	static bool shuffle;
	static bool zero;
	static bool reset;
	static bool stop;
	static bool shoot;
	static bool pistonS;
	static bool autoS;
	static bool shoot2;
	static bool compressor;
	static float topSpin;
	
	Task * m_ClawTask;
	
};
#endif
