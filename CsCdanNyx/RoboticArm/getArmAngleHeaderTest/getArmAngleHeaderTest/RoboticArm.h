// RoboticArm.h

#ifndef _ROBOTICARM_h
#define _ROBOTICARM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// Some parameters could be set
#define DegPrecision	1			// Prefered: 3 with speed 0.25, angSpeed 1. Angle's decimal precision.
#define DELAY	0					// Prefered: 0 by reason of servos' vibration (Due to intterupt triggering). Function servoAct's delaying.
#define CM2UNIT	10					// Defines how many coordinate units in 1 cm (1unit ~= 1mm).

// Arm's properties.


// Servo's Pin settings
#define servoPin0	2
#define servoPin1	3
#define servoPin2	4
#define servoPin3	5
#define servoPin4	6
#define servoPin5	7

// For calculation.
#define _USE_MATH_DEFINES
#define Rad2Degree 180/M_PI
// For printing
#define SIZEOF_ARRAY(array) (sizeof(array)/sizeof(*array))

#include <math.h>
#include "Servo.h"

class RoboticArmClass
{
 public:
	/*--------------------------Initializations----------------------------------*/
	//RoboticArmClass();
	void init(float ix = 220, float iy = 0, float iz = 360);
	
	///// Debugging ///////////////////////////
	void setJ(float * Ang);
	void servoInit();
	void servoDoJ();
	///////////////////////////////////////////

	/*----------------------Angle & Path Calculations----------------------------*/
	void getArmAngleDeg(float xp, float yp, float zp, float * Ang);
	void moveArmPath(float xd, float yd, float zd, float step = 1);	// step defines the distance(cm) arm moves in 1 step.

	//void adjPosition();				// Adjust position x, y, z value from Servos' angle J.
	//bool isAngleFailed();				// Check if Servos' angle hit the limitations.
	
	//void Jzero();

	/*-------------------------------Actions--------------------------------------*/
	void servoAct();													// Servos' signal output.
	void armGoTo(float xp, float yp, float zp);							// Arm move to point.
	void armGoLine(float xd, float yd, float zd, float step = 1);			// Move to destination linearly.
	void armGoDirect(float xd, float yd, float zd, float angSpeed = 1);		// Move to destination directly and angularly by changing angle per angSpeed degree.


	/*----------------------------Print and Show----------------------------------*/
	void showJ(const char * title = NULL);
	//void showAbJ(const char * title = NULL);
	void showXYZ(const char * title = NULL, bool XYZdisp = false);
	void printOut(float * AR, size_t ARsize, const char * Hstring = NULL, const char * split = ",");	//Print out array.
	void printOut(float n, const char * Hstring = NULL, const char * endString = "\n");					//Print out variable.

	float * getJ();
	float * getXYZ();

 private:

	float initDegree[6] = { 90, 90, 90, 90, 150 };
	float x = 0, y = 0, z = 0;						// Position coordinate.
	float J[6] = { 0, 0, 0, 0, 0, 135 };			// Each Servo's angle.
	bool parallelToFloor = true;					// Parallel to the ground, otherwise it would parallel to the whiteboard.
	float liftAngle = 0;							// Claw's lifting angle.
	Servo servoAR[6];

	
	// Arm's constant settings(mm).
	const float arm[4] = { 99, 159, 104.5f, 134.2f };		// Arms' length.
	//const float arm[5] = { 99, 134.2f, 159, 104.5f, 93.9875f };	// Arms' length.
	const float delX = 63.238f, delZ = 69.531f;				// Servos' coordinate compensations, while delta Y stay the same, P for parallel.
	const float delXP = 91.835f, delZP = 20;
	const float delYc = 4.5f;
	
};

extern RoboticArmClass Arm;

#endif

