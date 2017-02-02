#include "CompressorClass.h"

CompressorClass * CompressorClass::instance = NULL;
bool CompressorClass::compr = false;


DriverStationLCD * dsLCDCompr = DriverStationLCD::GetInstance();
void CompressorMonitor(CompressorClass * ctl) 
{
	AnalogChannel * PressureReader = new AnalogChannel((UINT8)1, (UINT32)3);
	
	Compressor * MyCompressor = NULL;

	bool disabled = false;
	float psi = 0.0;
	int psv = 0;
	float batVolt = 0.0;
	// Keep trying to create the compressor until we do it...
	while (1)
	{
		MyCompressor = new Compressor(1,(UINT32)1,1,(UINT32)1);
		if (MyCompressor != NULL)
		{
			MyCompressor->Start ();
			break;
		}
		Wait (1.0);			
	}

    // Manage the Compressor 
	while (1)
	{	
		disabled = ctl->GetCompr();
		psv = MyCompressor->GetPressureSwitchValue();
		psi = PressureReader->GetVoltage();
		batVolt = DriverStation::GetInstance()->GetBatteryVoltage();
		
		if(disabled)
		{
			MyCompressor->Stop();
		} else {
			MyCompressor->Start();
		}
		
		dsLCDCompr->Printf(DriverStationLCD::kUser_Line4 , 1, "Pressure: %f", psi);
		dsLCDCompr->UpdateLCD();
		
		//MyCompressor->SetRelayValue( MyCompressor->GetPressureSwitchValue() == 0 ? Relay::kOn : Relay::kOff );
		Wait (0.05);
			
	} // end while
}

CompressorClass::CompressorClass()
{
	    
	m_CompressorMonitor = new Task("3669Compressor", (FUNCPTR)CompressorMonitor, 100);
	m_CompressorMonitor->Start();
}

CompressorClass * CompressorClass::GetInstance()
{
	if(instance == NULL){
		instance = new CompressorClass();
	}
	return instance;
}

bool CompressorClass::GetCompr()
{
	bool output = false;
	output = compr;
	return output;
}

void CompressorClass::SetCompr(bool on)
{
	compr = on;
}





