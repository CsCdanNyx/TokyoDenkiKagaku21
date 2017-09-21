// RoboticArm.h

#ifndef _ROBOTICARM_h
#define _ROBOTICARM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

//Some parameters could be set
#define DegPrecision 3				// Angle's decimal precision.
#define DELAY 0					// Function servoAct's delaying.
#define Cm2Unit	1					// Defines how many coordinate units in 1 cm

#define _USE_MATH_DEFINES
#define Rad2Degree 180/M_PI

#include <math.h>
#include "Servo.h"

class RoboticArmClass
{
 public:
	/*--------------------------Initializations----------------------------------*/
	//RoboticArmClass(float ix, float iy, float iz);
	void init(float ix = 220, float iy = 0, float iz = 360);
	
	///// Debugging ///////////////////////////
	void setJ(float * Ang);
	void servoInit();
	void servoDoJ();
	///////////////////////////////////////////

	/*----------------------Angle & Path Calculations----------------------------*/
	void getArmAngleDeg(float xp, float yp, float zp, float * Ang);
	void moveArmPath(float xd, float yd, float zd, float step = 1);	// step defines the distance(cm) arm moves in 1 step.

	//void adjPosition();				//Adjust position x, y, z value from Servos' angle J.
	//bool isAngleFailed();				//Check if Servos' angle hit the limitations.
	
	//void Jzero();

	/*-------------------------------Actions--------------------------------------*/
	void servoAct();													// Servos' signal output.
	void armGoTo(float xp, float yp, float zp);							// Arm move to point.
	void armGoLine(float xd, float yd, float zd, float step = 1);			// Move to destination linearly.
	void armGoDirect(float xd, float yd, float zd, float angSpeed = 1);		// Move to destination directly by changing angle per angSpeed degree.


	/*----------------------------Print and Show----------------------------------*/
	void showJ();
	void showXYZ();

	float * getJ();
	float * getXYZ();

 private:

	float initDegree[6] = { 90, 90, 90, 90, 150 };

	float x = 0, y = 0, z = 0;						//Position coordinate.
	float deltay = 4.5f, deltaz = 0;				//Servos' coordinate compensations.
	float arm[4] = { 99, 159, 104.5f, 93.595f };	//Arms' length.
	bool parallel = true;							//Parallel to the ground.
	float J[6] = { 0, 0, 0, 0, 0, 135 };								//Each Servo's angle.

	Servo servoAR[6];
};

extern RoboticArmClass Arm;

#endif

