#pragma once

//Arduino incompatible
#include <string>

class RoboticArm
{
 public:
	//RoboticArmClass(float ix, float iy, float iz);
	void init(float ix, float iy, float iz);
	void getArmAngleDeg(float xp, float yp, float zp);
	//float * getArmPosition(float * angle);

	void moveArmPath(float xd, float yd, float zd, float speed);
	//void moveArmPathAdj(float xd, float yd, float zd, float speed, float unit);	// moveArmPath with continuous position adjustment after moving several units distance.

	//void adjPosition();				//Adjust position x, y, z value from Servos' angle J.
	//bool isAngleFailed();				//Check if Servos' angle hit the limitations.

	void moveArmPathPrint(float xd, float yd, float zd, float speed, std::string file = "../../../TempData/RbArmtest1.txt");

	float * getJ();
	void showJ();
	float * getXYZ();
	void showXYZ();
	//void Jzero();

 private:

	float x = 0, y = 0, z = 0;					//Position coordinate.
	float deltay = 4.5f, deltaz = 0;				//Servos' coordinate compensations.
	float arm[4] = { 99, 159, 104.5f, 93.595f };	//Arms' length.
	bool parallel = true;						//Parallel to the ground.
	float J[6] = { 0 };							//Each Servo's angle.
	int DegPrecision = 0;						//Angle's decimal precision.
};

extern RoboticArm Arm;

