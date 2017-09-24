// 
// 
// 

#include "RoboticArm.h"

/*
// Arm's constant settings(mm).
const float RoboticArmClass::arm[4] = { 99, 159, 104.5f, 134.2f };	// Arms' length.
//const float RoboticArmClass::arm[5] = { 99, 134.2f, 159, 104.5f,  };	// Arms' length.
const float RoboticArmClass::delX  = 63.238f, delZ  = 69.531f;		// Servos' coordinate compensations, while delta Y stay the same, P for parallel.
const float RoboticArmClass::delXP = 91.835f, delZP = 20;
const float RoboticArmClass::delYc = 4.5f;
*/

/*
RoboticArmClass::RoboticArmClass(float ix, float iy, float iz)
{
	const float RoboticArmClass::arm[4] = { 99, 159, 104.5f, 134.2f };	// Arms' length.
	const float RoboticArmClass::arm[5] = { 99, 134.2f, 159, 104.5f,  };	// Arms' length.
	const float RoboticArmClass::delX = 63.238f, delZ = 69.531f;		// Servos' coordinate compensations, while delta Y stay the same, P for parallel.
	const float RoboticArmClass::delXP = 91.835f, delZP = 20;
	const float RoboticArmClass::delYc = 4.5f;

	x = ix;
	y = iy;
	z = iz;
}
*/

/*--------------------------Initializations----------------------------------*/
void RoboticArmClass::init(float ix, float iy, float iz)
{	
	Serial.println("//----Start Initiallization----//");

	this->x = ix;
	this->y = iy;
	this->z = iz;

	servoAR[0].attach(servoPin0, 500, 2400);
	servoAR[1].attach(servoPin1, 500, 2400);
	servoAR[2].attach(servoPin2, 500, 2400);
	servoAR[3].attach(servoPin3, 500, 2400);
	servoAR[4].attach(servoPin4, 500, 2400);
	servoAR[5].attach(servoPin5, 500, 2400);

	armGoTo(x, y, z);
}


///// Debugging ///////////////////////////

void RoboticArmClass::setJ(float * Ang)
{
	for (size_t i = 0; i < 6; i++)
	{
		J[i] = Ang[i];
	}
}

void RoboticArmClass::servoInit()
{
	for (int i = 0; i < 6; i++) {
		servoAR[i].attach(i+2, 500, 2400);
	}
}

void RoboticArmClass::servoDoJ()
{
	for (int i = 0; i < 6; i++) {
		servoAR[i].write(J[i]);
	}
	if (DELAY)
		delay(DELAY);
}

///////////////////////////////////////////


/*----------------------Angle & Path Calculations----------------------------*/
void RoboticArmClass::getArmAngleDeg(float xp, float yp, float zp, float * Ang)
{
	/*	Begin calculations.  */

	// Ang[0], Ang[1]
	Ang[0] = asin((delYc + yp) / arm[0]);
	Ang[1] = -Ang[0];

	// Setting triangle () to calculate Ang[2], Ang[3]
	float rx = xp - arm[0] * cos(Ang[1]);
	float rz = zp - arm[3];
	if (parallelToFloor)
	{
		rx -= delXP;
		rz += delZP;
	}
	else
	{
		rx -= delX;
		rz -= delZ;
	}

	float r = sqrt(pow(rx, 2) + pow(rz, 2));
	float theta = atan(rz / rx);
	float a1 = r * cos(theta);
	float b1 = acos((pow(r, 2) + pow(arm[1], 2) - pow(arm[2], 2)) / (2 * arm[1] * r));

	Ang[2] = M_PI_2 - b1 - theta;

	// AsinIndCheck prevent asin from calculation over 1, which turns out to be -nan(ind), specified for yp == 65.
	float AsinIndCheck = (a1 - arm[1] * sin(Ang[2])) / arm[2];
	if (AsinIndCheck >= 1 && AsinIndCheck < 1.05)
		AsinIndCheck = 0.99999;

	Ang[3] = Ang[2] + M_PI_2 - asin(AsinIndCheck);

	if (parallelToFloor)
		Ang[4] = Ang[3] - Ang[2];
	else
		Ang[4] = Ang[3] - Ang[2] + M_PI / 3;

	for (size_t i = 0; i < 6; i++)
		Ang[i] = round(Ang[i] * Rad2Degree * pow(10, DegPrecision)) / pow(10, DegPrecision);

	Ang[1] = -Ang[1];
	Ang[2] = -Ang[2];
	Ang[3] = -Ang[3];
}

void RoboticArmClass::moveArmPath(float xd, float yd, float zd, float speed)
{
	float vec[3] = { xd - x, yd - y, zd - z };

	for (size_t i = 0; i < 3; i++)
		vec[i] = vec[i] / sqrt(pow(xd - x, 2) + pow(yd - y, 2) + pow(zd - z, 2)) * speed;

	while (x != xd || y != yd || z != zd)
	{
		getArmAngleDeg(x, y, z, J);

		this->x += vec[0];
		this->y += vec[1];
		this->z += vec[2];
	}
	getArmAngleDeg(xd, yd, zd, J);
	this->x = xd;
	this->y = yd;
	this->z = zd;
}


/*-------------------------------Actions--------------------------------------*/
void RoboticArmClass::armGoTo(float xp, float yp, float zp)
{
	getArmAngleDeg(xp, yp, zp, J);
	servoAct();

	///print
	//showJ();
	//showXYZ();
}

void RoboticArmClass::armGoLine(float xd, float yd, float zd, float step)
{
	float vec[3] = { xd - x, yd - y, zd - z };			// Vector point from src to destination.

	for (size_t i = 0; i < 3; i++)						// Step vector calculation.
		vec[i] = vec[i] / sqrt(pow(xd - x, 2) + pow(yd - y, 2) + pow(zd - z, 2)) * step * CM2UNIT/10;

	while (x != xd || y != yd || z != zd)
	{
		armGoTo(x, y, z);

		this->x += vec[0];
		this->y += vec[1];
		this->z += vec[2];

		//print
		//showJ();
		//showXYZ();
	}
	armGoTo(xd, yd, zd);

	this->x = xd;
	this->y = yd;
	this->z = zd;

	//print
	//showJ();
	//showXYZ();
}

void RoboticArmClass::armGoDirect(float xd, float yd, float zd, float angSpeed)
{
	float Ang[6];
	char sign[5] = { 1, 1, 1, 1, 1 };
	float tmp;

	getArmAngleDeg(xd, yd, zd, Ang);

	//print
	//printOut(Ang, SIZEOF_ARRAY(Ang), "Dest Ang:\t");
	//showXYZ();

	for (size_t i = 0; i < 5; i++)
	{
		tmp = Ang[i] - J[i];
		if (abs(tmp) <= angSpeed)
			sign[i] = 0;
		else if (tmp < 0)
			sign[i] = -1;
	}

	while (sign[0] || sign[1] || sign[2] || sign[3] || sign[4])
	{
		servoAct();
		for (size_t i = 0; i < 5; i++)
		{
			if (abs(Ang[i] - J[i]) <= angSpeed)
				sign[i] = 0;

			J[i] += sign[i] * angSpeed;
		}

		//print
		//showJ();
	}
	
	for (size_t i = 0; i < 6; i++)
		J[i] = Ang[i];

	servoAct();

	this->x = xd;
	this->y = yd;
	this->z = zd;

	//print
	//showJ("Final J: ");
	//showXYZ("Final XYZ: ");
}

void RoboticArmClass::servoAct()
{
	for (int i = 0; i < 6; i++)
	{
		if (DegPrecision)
			servoAR[i].writeMicroseconds(map((J[i] + initDegree[i]), 0, 180, 500, 2400));
		else
			servoAR[i].write(J[i] + initDegree[i]);
	}
	if (DELAY)
		delay(DELAY);
}

/*----------------------------Print and Show----------------------------------*/
void RoboticArmClass::showJ(const char * title)
{
	printOut(J, SIZEOF_ARRAY(J), title);
}

//void RoboticArmClass::showAbJ(const char * title)
//{
//	if (title)
//	{
//		Serial.print(String(title) + ": ");
//	}
//	Serial.print(String(AR[i], DegPrecision) + split + " ");
//	float Ang[6];
//	for (size_t i = 0; i < 6; i++)
//		Ang[i] = J[i] + initDegree[i];
//
//	printOut(Ang, SIZEOF_ARRAY(Ang), title);
//}

void RoboticArmClass::showXYZ(const char * title, bool XYZdisp)
{
	if (title)
		Serial.print(title);

	else if (XYZdisp)
		Serial.print("XYZ:\t");
	printOut(this->x, NULL, ", ");
	printOut(this->y, NULL, ", ");
	printOut(this->z, NULL);

	//if (XYZdisp)
	//{
	//	printOut(this->x, "x", ", ");
	//	printOut(this->y, "y", ", ");
	//	printOut(this->z, "z");
	//}
}

float * RoboticArmClass::getJ()
{
	return J;
}

float * RoboticArmClass::getXYZ()
{
	float P[3] = { x, y, z };
	return P;
}

void RoboticArmClass::printOut(float * AR, size_t ARsize, const char * Hstring, const char * split)
{
	if (Hstring)
		Serial.print(Hstring);

	for (size_t i = 0; i < ARsize-1; i++)
		Serial.print(String(AR[i], DegPrecision) + split + " ");
	Serial.print(String(AR[ARsize], DegPrecision) + "\n");
}

void RoboticArmClass::printOut(float n, const char * Hstring, const char * endString)
{
	if (Hstring)
		Serial.print(String(Hstring) + ": ");
	Serial.print(String(n, DegPrecision) + endString);
}

/*
void RoboticArmClass::Jzero()
{
	for (size_t i = 0; i < 6; i++)
		J[i] = 0;
}
*/


RoboticArmClass Arm;
