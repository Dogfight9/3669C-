#ifndef MYDEFINES_H
#define MYDEFINES_H

#define NULL      0

#define GYRO      1

#define RIGHT_DRIVE_MOTOR  2
#define LEFT_DRIVE_MOTOR   1

#define MAX_MOTOR_SPEED    0.3
#define JOYSTICK_DEADZONE  0.15

//XBOX 360 Button Mapping

#define A_BUTTON           1
#define B_BUTTON           2
#define X_BUTTON           3
#define Y_BUTTON           4
#define LEFT_BUMPER        5
#define RIGHT_BUMPER       6
#define BACK_BUTTON        7
#define START_BUTTON       8
#define LEFT_JOYSTICK      9
#define RIGHT_JOYSTICK     10
#define LEFT_STICK_X_AXIS  1
#define LEFT_STICK_Y_AXIS  2
#define TRIGGERS           3
#define RIGHT_STICK_X_AXIS 4
#define RIGHT_STICK_Y_AXIS 5


//Camera constants used for distance calculation
#define Y_IMAGE_RES 480		//X Image resolution in pixels, should be 120, 240 or 480
//#define VIEW_ANGLE 49		//Axis M1013
//#define VIEW_ANGLE 41.7		//Axis 206 camera
#define VIEW_ANGLE 37.4  //Axis M1011 camera
#define PI 3.141592653

//Score limits used for target identification
#define RECTANGULARITY_LIMIT 40
#define ASPECT_RATIO_LIMIT 55

//Score limits used for hot target determination
#define TAPE_WIDTH_LIMIT 50
#define VERTICAL_SCORE_LIMIT 50
#define LR_SCORE_LIMIT 50

//Minimum area of particles to be considered
#define AREA_MINIMUM 150

//Maximum number of particles to process
#define MAX_PARTICLES 8


#endif //MYDEFINES_H
