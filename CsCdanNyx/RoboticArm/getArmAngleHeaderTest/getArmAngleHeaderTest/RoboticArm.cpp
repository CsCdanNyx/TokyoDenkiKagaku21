// 
// 
// 

#include "RoboticArm.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define Rad2Degree 180/M_PI

/*
RoboticArmClass::RoboticArmClass(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
}
*/

void RoboticArmClass::init(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
}

void RoboticArmClass::getArmAngleDeg(float xp, float yp, float zp)
{
	float rx = 0, rz = 0;
	/*	Begin calculations.  */
	J[0] = asin((deltay + yp) / arm[0]);
	J[1] = -J[0];

	if (parallel)
	{
		deltaz = 134.2 - 20;
		rx = xp - arm[3] - arm[0] * cos(J[1]);
		rz = zp - deltaz;
	}
	else
		;

	float r = sqrt(pow(rx, 2) + pow(rz, 2));
	float theta = atan(rz / rx);
	float a1 = r * cos(theta);
	float b1 = acos((pow(r, 2) + pow(arm[1], 2) - pow(arm[2], 2)) / (2 * arm[1] * r));

	J[2] = M_PI_2 - b1 - theta;

	float AsinCheck = (a1 - 159 * sin(J[2])) / arm[2];
	if (AsinCheck >= 1 && AsinCheck < 1.05)		// Set for hit yp == 65 AsinCheck will be 1, which asin would return "-nan(ind)".
		AsinCheck = 0.99999;

	J[3] = J[2] + M_PI_2 - asin(AsinCheck);

	if (parallel)
		J[4] = J[3] - J[2];
	else
		J[4] = J[3] - J[2] + M_PI / 3;

	for (size_t i = 0; i < 6; i++)
		J[i] = round(J[i] * Rad2Degree * pow(10, DegPrecision)) / pow(10, DegPrecision);

	J[1] = -J[1];
	J[2] = -J[2];
	J[3] = -J[3];
}

void RoboticArmClass::moveArmPath(float xd, float yd, float zd, float speed)
{
	float vec[3] = { xd - x, yd - y, zd - z };

	for (size_t i = 0; i < 3; i++)
		vec[i] = vec[i] / sqrt(pow(xd - x, 2) + pow(yd - y, 2) + pow(zd - z, 2)) * speed;

	while (x != xd || y != yd || z != zd)
	{
		getArmAngleDeg(x, y, z);

		x = x + vec[0];
		y = y + vec[1];
		z = z + vec[2];
	}
	getArmAngleDeg(xd, yd, zd);
}

float * RoboticArmClass::getJ()
{
	return J;
}

/*
void RoboticArmClass::showJ()
{
	Serial.print("J:\t\t");
	for (size_t i = 0; i < 6; i++)
		Serial.print(" %.*f", DegPrecision, J[i]);
	Serial.print("\n");
}

float * RoboticArmClass::getPosition()
{
	float P[3] = { x, y, z };
	return P;
}

void RoboticArmClass::showPosition()
{
	Serial.print("Position:\t%.*f %.*f %.*f\n", DegPrecision, x, DegPrecision, y, DegPrecision, z);
}
*/

/*
void RoboticArmClass::Jzero()
{
	for (size_t i = 0; i < 6; i++)
		J[i] = 0;
}
*/


RoboticArmClass Arm;

