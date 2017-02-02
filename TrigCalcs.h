#ifndef TRIG_CALCS_H_
#define TRIG_CALCS_H_

#include "WPILib.h"
#include <math.h>

//Pi.  It needs to be more rational.
#define PI 3.1415926535897932384626433832795

//Joystick Bounds.  Currently 1.0f
#define BOUNDS 1.0f

//Wrapper for the atan2 function from math.h which returns degrees.
#define ATAN2DEG(y, x) (atan2((float)y, (float)x) * 180 / PI)

//Wrapper for the sine funtion from math.h which accepts degrees.
#define SINDEG(x) (sin(((float)x) * PI / 180))

//Wrapper for the cosine function from math.h which accepts degrees.
#define COSDEG(x) (cos(((float)x) * PI / 180))

//Wrapper for the tangent function from math.h which accepts degrees.
#define TANDEG(x) (tan(((float)x) * PI / 180))

//Cartesian to Polar coordinate function (returns polar distance from origin)
float cartToPolar(float inputX, float inputY);

//Calculate angle from origin based on coordinate
float angleCalc(float yFactor, float xFactor);

#endif
