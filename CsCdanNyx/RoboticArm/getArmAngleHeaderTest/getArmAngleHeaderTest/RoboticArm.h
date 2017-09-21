// RoboticArm.h

#ifndef _ROBOTICARM_h
#define _ROBOTICARM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define _USE_MATH_DEFINES
#define Rad2Degree 180/M_PI
#define DELAY 200

#include <math.h>
#include "Servo.h"

class RoboticArmClass
{
 public:
	/*--------------------------Initializations----------------------------------*/
	//RoboticArmClass(float ix, float iy, float iz);
	void init(float ix, float iy, float iz);

	/*----------------------Angle & Path Calculations----------------------------*/
	void getArmAngleDeg(float xp, float yp, float zp);
	void moveArmPath(float xd, float yd, float zd, float speed);

	//void adjPosition();				//Adjust position x, y, z value from Servos' angle J.
	//bool isAngleFailed();				//Check if Servos' angle hit the limitations.

	
	float * getJ();
	/*
	void showJ();
	float * getXYZ();
	void showXYZ();
	*/
	//void Jzero();

	/*-------------------------------Actions--------------------------------------*/
	void servoAct();
	void armGoTo(float xp, float yp, float zp);
	void armGoLine(float xd, float yd, float zd, float speed);



 private:

	float initDegree[6] = { 90, 90, 90, 90, 150, 90 };

	float x = 0, y = 0, z = 0;					//Position coordinate.
	float deltay = 4.5f, deltaz = 0;				//Servos' coordinate compensations.
	float arm[4] = { 99, 159, 104.5f, 93.595f };	//Arms' length.
	bool parallel = true;						//Parallel to the ground.
	float J[6] = { 0 };							//Each Servo's angle.
	int DegPrecision = 0;						//Angle's decimal precision.

	Servo servoAR[6];
};

extern RoboticArmClass Arm;

#endif

