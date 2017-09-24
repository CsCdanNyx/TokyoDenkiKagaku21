#include "stdafx.h"

#include "RoboticArm.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define Rad2Degree 180/M_PI

//Arduino incompatible
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void RoboticArm::init(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
}

void RoboticArm::getArmAngleDeg(float xp, float yp, float zp)
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

	float AsinCheck = (a1 - arm[1] * sin(J[2])) / arm[2];
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

//float * RoboticArm::getArmPosition(float * angle)
//{	
//	float _J[6];
//	float xyz[3];
//	memcpy(_J, angle, 6 * sizeof(float));
//	_J[1] = -_J[1];
//	_J[2] = -_J[2];
//	_J[3] = -_J[3];
//
//	for (size_t i = 0; i < 6; i++)
//		_J[i] = _J[i] / Rad2Degree;
//
//	float AsinCheck = sin(_J[2] - _J[3] + M_PI_2);
//	float a1 = AsinCheck * arm[2] + arm[1] * sin(_J[2]);
//	
//	xyz[1] = sin(_J[0]) * arm[0] - deltay;
//}

void RoboticArm::moveArmPath(float xd, float yd, float zd, float speed)
{
	float vec[3] = { xd - x, yd - y, zd - z };

	for (size_t i = 0; i < 3; i++)
		vec[i] = vec[i] / sqrt(pow(xd - x, 2) + pow(yd - y, 2) + pow(zd - z, 2)) * speed;

	while (x != xd || y != yd || z != zd)
	{
		getArmAngleDeg(x, y, z);
		showJ();

		x = x + vec[0];
		y = y + vec[1];
		z = z + vec[2];

	}
	getArmAngleDeg(xd, yd, zd);

}


void RoboticArm::moveArmPathPrint(float xd, float yd, float zd, float speed, std::string file)//, string file
{
	ofstream f;
	f.open(file);

	float * degree;

	float vec[3] = { xd - x, yd - y, zd - z };

	for (size_t i = 0; i < 3; i++)
		vec[i] = vec[i] / sqrt(pow(xd - x, 2) + pow(yd - y, 2) + pow(zd - z, 2)) * speed;

	while (x != xd || y != yd || z != zd)
	{
		getArmAngleDeg(x, y, z);

		degree = getJ();

		for (size_t i = 0; i < 6; i++)
			f << degree[i] << ',';
		f << endl;

		x = x + vec[0];
		y = y + vec[1];
		z = z + vec[2];
	}
	getArmAngleDeg(xd, yd, zd);
	
	for (size_t i = 0; i < 6; i++)
		f << degree[i] << ',';
	f << endl;

}

float * RoboticArm::getJ()
{
	return J;
}

void RoboticArm::showJ()
{
	printf("J:\t\t");
	for (size_t i = 0; i < 6; i++)
		printf(" %.*f", DegPrecision, J[i]);
	printf("\n");
}

float * RoboticArm::getXYZ()
{
	float P[3] = { x, y, z };
	return P;
}

void RoboticArm::showXYZ()
{
	printf("Position:\t%.*f %.*f %.*f\n", DegPrecision, x, DegPrecision, y, DegPrecision, z);
}

/*
void RoboticArm::Jzero()
{
for (size_t i = 0; i < 6; i++)
J[i] = 0;
}
*/


RoboticArm Arm;
