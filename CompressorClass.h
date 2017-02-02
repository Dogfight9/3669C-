#ifndef CompressorClass_H
#define CompressorClass_H
#pragma once

#include "MyDefines.h"
#include "WPILib.h"

class CompressorClass
{
public:
	static CompressorClass * GetInstance();
	void SetCompr(bool on);
	bool GetCompr();

	
protected:
	CompressorClass();
	
private:
	static CompressorClass * instance;
	static bool compr;
	
	
		
	Task * m_CompressorMonitor;
};
#endif // Compressor_H
