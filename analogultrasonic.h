#include "WPILib.h"

#define IN_TO_CM_CONVERSION 2.54

/* AnalogUltrasonic class
 * For use with analog ultrasonic sensors
 * Capabilities:
 * 		Return voltage
 * 		Return range in either inches or centimeters if the constructor is given correct data
 * Averaging and oversampling are not built in (yet)
 */
class AnalogUltrasonic : public SensorBase {
private:
	bool use_units;			//Are we using units or just returning voltage?
	float min_voltage;		//Minimum voltage the ultrasonic sensor can return
	float voltage_range;	//The range of the voltages returned by the sensor (maximum - minimum)
	float min_distance;		//Minimum distance the ultrasonic sensor can return in inches
	float distance_range;	//The range of the distances returned by this class in inches (maximum - minimum)
	AnalogChannel *channel;
public:
/* constructor
 * @param channel A pointer to the AnalogChannel object corresponding to the proper channel and ports
 * @param port The port on the analog module the ultrasonic sensor is plugged into
 * @param _use_units Are we using units or just giving voltages?
 * @param _min_voltage The voltage the ultrasonic sensor gives at the minimum range (if units are used)
 * @param _max_voltage The voltage the ultrasonic sensor gives at the maximum range (if units are used)
 * @param _min_distance The minimum range the ultrasonic sensor can give in inches (if units are used)
 * @param _max_distanceThe maximum range the ultrasonic sensor can give in inches (if units are used)
 */
	AnalogUltrasonic(AnalogChannel *_channel, bool _use_units, float _min_voltage, float _max_voltage, float _min_distance, float _max_distance);
// Just get the voltage.
	float GetVoltage();
/* GetRangeInInches
 * Returns the range in inches
 * Returns -1.0 if units are not being used
 * Returns -2.0 if the voltage is below the minimum voltage
 */
	float GetRangeInInches();
/* GetRangeInCM
 * Returns the range in centimeters
 * Returns -1.0 if units are not being used
 * Returns -2.0 if the voltage is below the minimum voltage
 */
	float GetRangeInCM();
};
