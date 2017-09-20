// RoboticArm.h

#ifndef _ROBOTICARM_h
#define _ROBOTICARM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class RoboticArmClass
{
 protected:

 public:
	//RoboticArmClass(float ix, float iy, float iz);
	void init(float ix, float iy, float iz);
	void getArmAngleDeg(float xp, float yp, float zp);
	void moveArmPath(float xd, float yd, float zd, float speed);

	//void adjPosition();				//Adjust position x, y, z value from Servos' angle J.
	//bool isAngleFailed();				//Check if Servos' angle hit the limitations.
	
	float * getJ();
	void showJ();
	float * getPosition();
	void showPosition();
	//void Jzero();
 private:
	float x = 0, y = 0, z = 0;					//Position coordinate.
	float deltay = 4.5, deltaz = 0;				//Servos' coordinate compensations.
	float arm[4] = { 99, 159, 104.5, 93.595 };	//Arms' length.
	bool parallel = true;						//Parallel to the ground.
	float J[6] = { 0 };							//Each Servo's angle.
	int DegPrecision = 0;						//Angle's decimal precision.
};

extern RoboticArmClass Arm;

#endif

