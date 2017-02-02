#include "TrigCalcs.h"

float cartToPolar(float inputX, float inputY)
{
	//Convert x-y cartesian coordinate to a polar distance
	//from center.

	float edgePointX, edgePointY;

	//First calculate the slope
	//if one of the values is 0, respond appropriately so that
	//we don't have divide by 0 errors and such
	if(inputY == 0)
	{
		//If there is no Y factor
		if(inputX > 0)
			edgePointX = BOUNDS;
		else if(inputX < 0)
			edgePointX = -BOUNDS;
		else
			//Return 0 if both x and y inputs are 0
			return 0;
		
		edgePointY = 0;
	}
	else if(inputX == 0)
	{
		//If there is no X factor
		if(inputY > 0)
			edgePointY = BOUNDS;
		else if(inputY < 0)
			edgePointY = -BOUNDS;
		else
			//Return 0 if both x and y inputs are 0
			return 0;

		edgePointX = 0;
	}
	else
	{
		//If we have no 0 in x or y input, calculate slope
		//normally
		float slope = inputY / inputX;

		//Project a point along the line of our actual point
		//using the slope	

		if(slope >= 1.0f || slope <= -1.0f)
		{
			//If the point is on the top or bottom bounds
			edgePointX = BOUNDS / slope;
			edgePointY = BOUNDS;
		}
		else
		{
			//If the point is on the side bounds
			edgePointX = BOUNDS;
			edgePointY = BOUNDS * slope;
		}
	}

	//Calculate the length of the line from the center
	//to our edge point
	float edgeDistance = sqrt(pow(fabs(edgePointX), 2) + pow(fabs(edgePointY), 2));

	//Calculate length of line from the center to our actual point
	float inputDistance = sqrt(pow(((float)fabs(inputX)), 2) + pow(((float)fabs(inputY)),2));

	//Return the "polar distance" by finding the ratio of these two lines
	//and multiplying by the bound
	return BOUNDS * (inputDistance / edgeDistance);
}

float angleCalc(float yFactor, float xFactor)
{
	//calculate the angle off of the right intercept

	if(yFactor > 0)
	//Quadrants 1 and 2
	{
		return ATAN2DEG(yFactor, xFactor);
	}
	else
	//Quadrants 3 and 4
	{
		return (ATAN2DEG(yFactor, xFactor)) + 360;
	}
}
