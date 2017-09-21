// 
// 
// 

#include "RoboticArm.h"
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
	this->x = ix;
	this->y = iy;
	this->z = iz;

	for (int i = 0; i<6;) {
		servoAR[i].attach(++i, 500, 2400);
	}
	armGoTo(x, y, z);
}

void RoboticArmClass::getArmAngleDeg(float xp, float yp, float zp, float * Ang)
{
	float rx = 0, rz = 0;
	/*	Begin calculations.  */
	Ang[0] = asin((deltay + yp) / arm[0]);
	Ang[1] = -Ang[0];

	if (parallel)
	{
		deltaz = 134.2 - 20;
		rx = xp - arm[3] - arm[0] * cos(Ang[1]);
		rz = zp - deltaz;
	}
	else
		;

	float r = sqrt(pow(rx, 2) + pow(rz, 2));
	float theta = atan(rz / rx);
	float a1 = r * cos(theta);
	float b1 = acos((pow(r, 2) + pow(arm[1], 2) - pow(arm[2], 2)) / (2 * arm[1] * r));

	Ang[2] = M_PI_2 - b1 - theta;

	float AsinCheck = (a1 - arm[1] * sin(Ang[2])) / arm[2];
	if (AsinCheck >= 1 && AsinCheck < 1.05)		// Set for hit yp == 65 AsinCheck will be 1, which asin would return "-nan(ind)".
		AsinCheck = 0.99999;

	Ang[3] = Ang[2] + M_PI_2 - asin(AsinCheck);

	if (parallel)
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

void RoboticArmClass::servoAct()
{
	for (int i = 0; i < 6; i++) {
		servoAR[i].write(J[i] + initDegree[i]);
	}
	if (DELAY)
		delay(DELAY);
}

void RoboticArmClass::armGoTo(float xp, float yp, float zp)
{
	getArmAngleDeg(xp, yp, zp, J);
	servoAct();

	///print
	showJ();
}

void RoboticArmClass::armGoLine(float xd, float yd, float zd, float step)
{
	float vec[3] = { xd - x, yd - y, zd - z };			// Vector point from src to destination.

	for (size_t i = 0; i < 3; i++)						// Step vector calculation.
		vec[i] = vec[i] / sqrt(pow(xd - x, 2) + pow(yd - y, 2) + pow(zd - z, 2)) * step * Cm2Unit;

	while (x != xd || y != yd || z != zd)
	{
		armGoTo(x, y, z);

		this->x += vec[0];
		this->y += vec[1];
		this->z += vec[2];
	}
	armGoTo(xd, yd, zd);

	this->x = xd;
	this->y = yd;
	this->z = zd;
}

void RoboticArmClass::armGoDirect(float xd, float yd, float zd, float angSpeed)
{
	float Ang[6];
	char sign[6] = { 1 };

	getArmAngleDeg(xd, yd, zd, Ang);

	for (size_t i = 0; i < 6; i++)
	{
		Ang[i] = Ang[i] - J[i];
		if (abs(Ang[i]) <= angSpeed)
			sign[i] = 0;
		else if (Ang[i] < 0)
			sign[i] = -1;
	}

	while (sign[0] || sign[1] || sign[2] || sign[3] || sign[4] || sign[5])
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (abs(Ang[i] - J[i]) <= angSpeed)
				sign[i] = 0;

			J[i] += sign[i] * angSpeed;
		}
		servoAct();
	}
	
	for (size_t i = 0; i < 6; i++)
	{
		J[i] = Ang[i];
	}
	servoAct();

}


void RoboticArmClass::showJ()
{
	//Serial.print("J:\t\t");
	for (size_t i = 0; i < 6; i++)
	{
		Serial.print(int(J[i]), DEC);
		if (DegPrecision)
		{
			Serial.print(".");
			Serial.print(int(J[i] * pow(10, DegPrecision)), DEC);
		}
		Serial.print(", ");
	}
	Serial.println("");
}

void RoboticArmClass::showXYZ()
{
	Serial.print("Position: ");
	if (DegPrecision)
	{
		Serial.print(int(x), DEC);
		Serial.print(", ");
		Serial.print(int(y), DEC);
		Serial.print(", ");
		Serial.print(int(z), DEC);
	}
	else
	{
		Serial.print(int(x), DEC);
		if (DegPrecision)
		{
			Serial.print(".");
			Serial.print(int(x * pow(10, DegPrecision)), DEC);
			Serial.print(", ");
		}

		Serial.print(int(y), DEC);
		if (DegPrecision)
		{
			Serial.print(".");
			Serial.print(int(y * pow(10, DegPrecision)), DEC);
			Serial.print(", ");
		}

		Serial.print(int(z), DEC);
		if (DegPrecision)
		{
			Serial.print(".");
			Serial.print(int(z * pow(10, DegPrecision)), DEC);
		}
	}
	
	Serial.println("");
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


/*
void RoboticArmClass::Jzero()
{
	for (size_t i = 0; i < 6; i++)
		J[i] = 0;
}
*/


RoboticArmClass Arm;

