// 
// 
// 

#include "RoboticArm.h"


static volatile boolean objectDetect = 0;

///*----------------------------Arm's settings----------------------------------*/
//RoboticArmClass::RoboticArmClass()
//{
//
//}


/*--------------------------Initializations----------------------------------*/
void RoboticArmClass::init(float ix, float iy, float iz)
{	
	Serial.println("//----Start Initiallization----//");

	this->x = ix;
	this->y = iy;
	this->z = iz;

	pinMode(detect_optic, INPUT);


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


	// Setting up triangle ( point: J2,J3,J4 which is arm[2],arm[3],r ) for calculating Ang[2], Ang[3]
	float rx = xp - arm[4] * cos(arm4ToXYang + tiltAngle) - arm[0] * cos(Ang[1]);
	float rz = zp - arm[1] - arm[4] * sin(arm4ToXYang + tiltAngle);
	float r = sqrt(pow(rx, 2) + pow(rz, 2)); // r is the line between the point J2-J4.

	// Ang[2]
	Ang[2] = M_PI_2 - ( acos((pow(r, 2) + pow(arm[2], 2) - pow(arm[3], 2)) / (2 * arm[2]* r)) ) - atan(rz / rx);
	// J[2] = pi/2 - ( The angle between r and arm[2] ) - ( The angle between r and XY plane )


	// Ang[3]
	// AsinIndCheck prevent asin from calculation over 1, which turns out to be -nan(ind), specified for yp == 65.
	float AsinIndCheck = (rx - arm[2] * sin(Ang[2])) / arm[3];
	if (AsinIndCheck >= 1)
	{
		Serial.println(String("AsinIndCheck error, when") + " x:" + xp + " y:" + yp + " z:" + zp + " AsinIn..k:" + String(AsinIndCheck, 8));
		AsinIndCheck = 0.99999;
	}

	Ang[3] = -Ang[2] + M_PI_2 - asin(AsinIndCheck);

	// Ang[4]
	Ang[4] = -Ang[3] - Ang[2] + tiltAngle;

	// Rounding
	for (size_t i = 0; i < 5; i++)
		Ang[i] = round(Ang[i] * Rad2Degree * pow(10, DegPrecision)) / pow(10, DegPrecision);

	Ang[1] = -Ang[1];
	Ang[2] = -Ang[2];
	Ang[3] = Ang[3];
}

// Still in process
void RoboticArmClass::getArmPosition(float * Ang, float * XYZ)
{

}


/*-------------------------------Actions--------------------------------------*/
void RoboticArmClass::servoAct()
{
	for (int i = 0; i < 6; i++)
	{
		if (DegPrecision)
			servoAR[i].writeMicroseconds(map(((J[i] + initDegree[i]) * pow(10, DegPrecision)), 0, (180 * pow(10, DegPrecision)), 500, 2400));
		else
			servoAR[i].write(J[i] + initDegree[i]);
	}
	if (DELAY)
		delay(DELAY);

}
/**-----------------------Arm--------------------------------**/
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
		vec[i] = vec[i] / sqrt(pow(xd - x, 2) + pow(yd - y, 2) + pow(zd - z, 2)) * step * CM2UNIT / 10;

	while (x != xd || y !=yd || z !=zd)
	{
		armGoTo(x, y, z);
		if (abs(x - xd) >= 0.5)
			this->x += vec[0];
		else
			this->x = xd;
		if (abs(y - yd) >= 0.5)
			this->y += vec[1];
		else
			this->y = yd;
		if (abs(z - zd) >= 0.5)
			this->z += vec[2];
		else
			this->z = zd;

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
	float Ang[5];
	char sign[5] = { 1, 1, 1, 1, 1 };
	float tmp;

	getArmAngleDeg(xd, yd, zd, Ang);

	//print
	printOut(Ang, SIZEOF_ARRAY(Ang), "Dest Ang:\t");
	showXYZ();

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

	for (size_t i = 0; i < 5; i++)
		J[i] = Ang[i];

	servoAct();

	this->x = xd;
	this->y = yd;
	this->z = zd;

	//print
	//showJ("Final J: ");
	//showXYZ("Final XYZ: ");
}
/**-----------------------Claw--------------------------------**/
void RoboticArmClass::clawClamp(float * Ang, char RelvClp)
{
	if (RelvClp == 'r')
	{
		///print
		//Serial.println("Release!!");
		Ang[5] = 90;
	}
	else
	{
		///print
		//Serial.println("Clamp!!");
		Ang[5] = 130;
	}

	servoAct();
}

/*-------------------------------Challenge--------------------------------------*/
/**------------------Grab Marker Pen-------------------------**/
static void DetectDistance(void)
{
	if (digitalRead(detect_optic))        // If detector Output is HIGH,
	{
		objectDetect = false;           // then no object was detected;
	}
	else                                // but if the Output is LOW,
	{
		delayMicroseconds(395);               // wait for another 15 pulses.
		if (digitalRead(detect_optic))    // If the Output is now HIGH,
		{                               // then first Read was noise
			objectDetect = false;       // and no object was detected;
		}
		else                            // but if the Output is still LOW,
		{
			objectDetect = true;        // then an object was truly detected.
		}
	}
}



int RoboticArmClass::GrabPen(float penX, float penY, float penZ, float speed)
{
	int initxyz[3] = { x,y,z };
	int liftPenHeight = 110;
	Serial.println("Release");
	clawClamp(J,'r');
	armGoLine( (x + 50), penY, z, speed);
	armGoLine(x, penY, penZ, speed);
	Timer1.attachInterrupt(DetectDistance, 210);

	for (float count = (penX - x) / 100; count <= (penX - x); count = count + count) {
		armGoLine((x + count), penY, penZ, speed);
		if (objectDetect) break;
	}
	Timer1.detachInterrupt();

	clawClamp(J,'g');
	armGoLine(penX, penY, (penZ + liftPenHeight), speed);
	armGoLine(initxyz[0],  initxyz[1], initxyz[2], speed);
	return 1;	// In case of slides or controller needs the return value.
}


/**----------------------Writing-----------------------------**/
void RoboticArmClass::LiftPen(float * Ang, char UpvDn, float penliftAng)
{
	if (UpvDn == 'u')
		Ang[4] += penliftAng;
	else
		Ang[4] -= penliftAng;

	servoAct();
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
	Serial.print(String(AR[ARsize-1], DegPrecision) + "\n");
}

void RoboticArmClass::printOut(float n, const char * Hstring, const char * endString)
{
	if (Hstring)
		Serial.print(String(Hstring) + ": ");
	Serial.print(String(n, DegPrecision) + endString);
}

/**-----------Unused--------------*/
/*
void RoboticArmClass::Jzero()
{
	for (size_t i = 0; i < 6; i++)
		J[i] = 0;
}
*/

//void RoboticArmClass::moveArmPath(float xd, float yd, float zd, float speed)
//{
//	float vec[3] = { xd - x, yd - y, zd - z };
//
//	for (size_t i = 0; i < 3; i++)
//		vec[i] = vec[i] / sqrt(pow(xd - x, 2) + pow(yd - y, 2) + pow(zd - z, 2)) * speed;
//
//	while (x != xd || y != yd || z != zd)
//	{
//		getArmAngleDeg(x, y, z, J);
//
//		this->x += vec[0];
//		this->y += vec[1];
//		this->z += vec[2];
//	}
//	getArmAngleDeg(xd, yd, zd, J);
//	this->x = xd;
//	this->y = yd;
//	this->z = zd;
//}


RoboticArmClass Arm;