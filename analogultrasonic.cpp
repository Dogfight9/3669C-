#include "analogultrasonic.h"


//constructor
AnalogUltrasonic::AnalogUltrasonic(AnalogChannel *_channel, bool _use_units = 0, float _min_voltage = 0.0, float _max_voltage = 5.0, float _min_distance = 3.0, float _max_distance = 60.0) {
	if(_channel == NULL)
		;//wpi_fatal(NullParameter);
	else
		channel = _channel;
	//only use unit-specific variables if we're using units
	if(_use_units) {
		use_units = 1;
		min_voltage = _min_voltage;
		voltage_range = _max_voltage - _min_voltage;
		min_distance = _min_distance;
		distance_range = _max_distance - _min_distance;
	}
}
// Just get the voltage.
float AnalogUltrasonic::GetVoltage() {
	float range = channel->GetVoltage();
	
	return range;
}
/* GetRangeInInches
* Returns the range in inches
* Returns -1.0 if units are not being used
* Returns -2.0 if the voltage is below the minimum voltage
*/
float AnalogUltrasonic::GetRangeInInches() {
	float range;
	//if we're not using units, return -1, a range that will most likely never be returned
	if(!use_units){
		return -1.0;
	}
	range = channel->GetVoltage();
	if(range < min_voltage){
		return -2.0;
	}
	//first, normalize the voltage
	range = (range - min_voltage) / voltage_range;
	//next, denormalize to the unit range
	range = (range * distance_range) + min_distance;
	return range;
}
/* GetRangeInCM
* Returns the range in centimeters
* Returns -1.0 if units are not being used
* Returns -2.0 if the voltage is below the minimum voltage
*/
float AnalogUltrasonic::GetRangeInCM() {
	float range;
	//if we're not using units, return -1, a range that will most likely never be returned
	if(!use_units)
		return -1.0;
	range = channel->GetVoltage();
	if(range < min_voltage)
		return -2.0;
	//first, normalize the voltage
	range = (range - min_voltage) / voltage_range;
	//next, denormalize to the unit range
	range = (range * distance_range) + min_distance;
	//finally, convert to centimeters
	range *= IN_TO_CM_CONVERSION;
	return range;
}
