//
//
//

#include "RoboticArm.h"

/*--------------------------Initializations----------------------------------*/
void RoboticArmClass::initServo()
{
#ifdef DEBUG
	Serial.println("//----Start Initialization----//");
#endif // DEBUG

	servoAR[0].attach(servoPin0, 500, 2400);
	servoAR[1].attach(servoPin1, 500, 2400);
	servoAR[2].attach(servoPin2, 500, 2400);
	servoAR[3].attach(servoPin3, 500, 2400);
	servoAR[4].attach(servoPin4, 500, 2400);
	servoAR[5].attach(servoPin5, 500, 2400);

	servoAct();
}

int RoboticArmClass::initPosit(float ix, float iy, float iz, float angSpeed)
{

	initXYZ[0] = ix;
	initXYZ[1] = iy;
	initXYZ[2] = iz;
	armGoDirect(ix, iy, iz, angSpeed);

	return 1;
}

/*----------------------Angle & Path Calculations----------------------------*/
void RoboticArmClass::getArmAngleDeg(float xp, float yp, float zp, float * Deg, bool setError)
{
	float Ang[6] = { 0 };
	Ang[5] = Deg[5];
	/*	Begin calculations.  */
	// Ang[0], Ang[1]
	Ang[0] = asin((delYc + yp) / arm[0]);
	Ang[1] = -Ang[0];

	// Setting up triangle ( point: J2,J3,J4 which is arm[2],arm[3],r ) for calculating Ang[2], Ang[3]
	float rx = xp - arm[4] * cos(arm4ToXYang + tiltAngle) - arm[0] * cos(Ang[1]);
	float rz = zp - arm[1] - arm[4] * sin(arm4ToXYang + tiltAngle);
	float r = sqrt(pow(rx, 2) + pow(rz, 2)); // r is the line between the point J2-J4.
	/*check if r <= all arm*/

	// Ang[2]
	Ang[2] = M_PI_2 - (acos((pow(r, 2) + pow(arm[2], 2) - pow(arm[3], 2)) / (2 * arm[2] * r))) - atan(rz / rx);
	// J[2] = pi/2 - ( The angle between r and arm[2] ) - ( The angle between r and XY plane )

	// Ang[3]
	// AsinIndCheck prevent asin from calculation over 1, which turns out to be -nan(ind), specified for yp == 65.
	float AsinIndCheck = (rx - arm[2] * sin(Ang[2])) / arm[3];


	if (AsinIndCheck >= 1)
	{
#ifdef ErrorOut
		Serial.println(String("AsinIndCheck error, when") + " x:" + xp + " y:" + yp + " z:" + zp + " AsinIn..k:" + String(AsinIndCheck, 8));
#endif // ErrorOut
		this->ERRcode = -2;
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

	memcpy(Deg, Ang, 5 * sizeof(float));

}

bool RoboticArmClass::isAngExcess(float * Ang, bool setError)
{
	for (size_t i = 0; i < 6; i++)
	{
		if ((baseDegree[i] + Ang[i]) > 180 || (baseDegree[i] + Ang[i]) < 0)
		{
			if (setError)
				this->ERRcode = 1;
			return true;
		}
	}

	return false;
}

/*-------------------------------Actions--------------------------------------*/
void RoboticArmClass::servoAct(bool ERRcheck)
{
#ifdef	ErrorHandle
	if (ERRcheck)
	{
		isAngExcess(J, true);
		ArmErrorHandle();
	}
#endif // ErrorHandle

	for (int i = 0; i < 6; i++)
	{
		if (DegPrecision)
			servoAR[i].writeMicroseconds(map(((J[i] + baseDegree[i]) * pow(10, DegPrecision)), 0, (180 * pow(10, DegPrecision)), 500, 2400));
		else
			servoAR[i].write(J[i] + baseDegree[i]);
	}
#if SERVODELAY
	delay(SERVODELAY);
#endif // SERVODELAY

}

/**-----------------------Arm--------------------------------**/
void RoboticArmClass::armGoTo(float xp, float yp, float zp)
{
	getArmAngleDeg(xp, yp, zp, J, true);
	servoAct(false);

	///print
#ifdef DEBUG
	//showJ();
	//showXYZ();
#endif // DEBUG

}

void RoboticArmClass::armGoLine(float xd, float yd, float zd, float step)
{
	float vec[3] = { xd - this->x, yd - this->y, zd - this->z };			// Vector point from src to destination.

	for (size_t i = 0; i < 3; i++)						// Step vector calculation.
		vec[i] = vec[i] / sqrt(pow(xd - this->x, 2) + pow(yd - this->y, 2) + pow(zd - this->z, 2)) * step * CM2UNIT / 10;

	while (this->x != xd || this->y != yd || this->z != zd)
	{
		armGoTo(this->x, this->y, this->z);

		if (abs(x - xd) >= abs(vec[0]))	this->x += vec[0];
		else		this->x = xd;

		if (abs(y - yd) >= abs(vec[1]))	this->y += vec[1];
		else		this->y = yd;

		if (abs(z - zd) >= abs(vec[2]))	this->z += vec[2];
		else		this->z = zd;

		///print
#ifdef DEBUG
		//showJ();
		//showXYZ();
#endif // DEBUG
	}
	armGoTo(this->x, this->y, this->z);

	///print
#ifdef DEBUG
	//showJ();
	//showXYZ();
#endif // DEBUG

}

void RoboticArmClass::armGoDirect(float xd, float yd, float zd, float angSpeed)
{
	float Ang[6] = { 0 };
	char sign[5] = { 1, 1, 1, 1, 1 };
	float tmp;

	getArmAngleDeg(xd, yd, zd, Ang, true);

#ifdef DEBUG
	//printOut(Ang, SIZEOF_ARRAY(Ang), "Dest Ang:\t");
	//showXYZ();
#endif // DEBUG

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
		servoAct(false);
		for (size_t i = 0; i < 5; i++)
		{
			if (abs(Ang[i] - J[i]) <= angSpeed)
				sign[i] = 0;

			J[i] += sign[i] * angSpeed;

			// Error checking
			if ((baseDegree[i] + J[i]) > 180 || (baseDegree[i] + J[i]) < 0)
			{
				this->ERRcode = 1;
			}

		}

		///print
#ifdef DEBUG
		//showJ();
		//showXYZ();
#endif // DEBUG

	}

	memcpy(J, Ang, 5 * sizeof(float));

	servoAct(false);

	this->x = xd;
	this->y = yd;
	this->z = zd;

	///print
#ifdef DEBUG
	showJ("Final J: ");
	//showXYZ("Final XYZ: ");
#endif // DEBUG

}

/***--------------------Overload---------------------***/
void RoboticArmClass::armGoLine(float desXYZ[3], float step)
{
	armGoLine(desXYZ[0], desXYZ[1], desXYZ[2], step);
}

void RoboticArmClass::armGoDirect(float desXYZ[3], float angSpeed)
{
	armGoDirect(desXYZ[0], desXYZ[1], desXYZ[2], angSpeed);
}

/**-----------------------Claw--------------------------------**/
void RoboticArmClass::clawClamp(float * Ang, char RelvClp)
{
	if (RelvClp == 'r')
	{

		///print
#ifdef DEBUG
		//Serial.println("Release!!");
#endif // DEBUG
		Ang[5] = 0;
	}
	else
	{
		///print
#ifdef DEBUG
		//Serial.println("Clamp!!");
#endif // DEBUG
		Ang[5] = 50;
	}

	servoAct();
}

/*-------------------------------Challenge--------------------------------------*/
/**------------------Grab Marker Pen-------------------------**/
int RoboticArmClass::GrabPen(float penX, float penY, float penZ, float step, float angspeed)
{

#ifdef DEBUG
	Serial.println("Start GrabPen!!");
#endif // DEBUG
	int liftPenHeight = 75;
	float offsetY = 3;
	float offsetX = 13;
	float offsetZforfarX = 10;
	float GrabPtUpperBoundfromEnd = 1;
	bool notDetected = true;		// Check if optic detection detect pen. If detected, return false.
#ifdef DEBUG
	Serial.println("Release");
#endif // DEBUG
	armGoDirect(initXYZ, angspeed);
	clawClamp(J, 'r');

	// Initial position for optic detection
	armGoDirect(this->x, (this->y + 70), penZ + 10, angspeed);
#ifdef DEBUG
	Serial.println("Posit ready for grab!!");
#endif // DEBUG
	delay(500);

	// Detect pen alone y-axis
	pinMode(OPTIC_ENABLE_Y_PIN, OUTPUT);
	digitalWrite(OPTIC_ENABLE_Y_PIN, HIGH);
	pinMode(OPTIC_Y_INPUT_PIN, INPUT);
	delay(500);
	
	for (float i = 0; i < 140; i += 0.5f)
	{
		armGoLine(this->x, this->y - 0.5f, this->z, step);
		notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
		if (!notDetected) {
			delay(OPTIC_DELAY);
			notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
			if (!notDetected)
			{
#ifdef DEBUG
				Serial.println("interruptY");
				//showXYZ("interruptY\n");
#endif // DEBUG
				break;
			}
		}
	}

	// Second y detection if hasn't detected yet
	if (notDetected)
	{
		delay(500);
		for (float i = 0; i < 140; i += 0.5f)
		{
			armGoLine(this->x, this->y + 0.5f, this->z, step);
			notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
			if (!notDetected) {
				delay(OPTIC_DELAY);
				notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
				if (!notDetected)
				{
#ifdef DEBUG
					Serial.println("2ndInterruptY");
					//showXYZ("2ndInterruptY\n");
#endif // DEBUG
				}
			}
		}
	}

	if (notDetected)
	{
		delay(500);
		armGoLine(this->x, this->y - 70, this->z, step);
	}

#ifdef DEBUG
	//Serial.println("Y finished!!");
	//showXYZ("Y finished!\n");
	//showJ("InY:\t");
#endif // DEBUG

	// Detect the end of the pen alone z-axis
	delay(1000);
	for (size_t i = 0; i < 200; i++)
	{
		armGoLine(this->x, this->y, this->z + 0.5, step / 2);
		notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
#ifdef DEBUG
		//Serial.println(notDetected?"not":"yes");
#endif // DEBUG			
		if (notDetected)
		{
			delay(OPTIC_DELAY);
			notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
			if (notDetected)
			{
#ifdef DEBUG
				Serial.println("End of the Pen at Z.");
				//showJ("EndZ:\t");
#endif // DEBUG					
				
				armGoLine(this->x, this->y, this->z - 0.5 - GrabPtUpperBoundfromEnd, step / 2);
				break;
			}
		}
	}

	digitalWrite(OPTIC_ENABLE_Y_PIN, LOW);

#ifdef DEBUG	
	Serial.println("Z finished!!");
	//showJ("CatchZ:\t");
#endif // DEBUG	

	// y-axis compensate positioning
	armGoLine(this->x, (this->y > (70 - offsetY) ? this->y : (this->y + offsetY)), this->z, step / 2);		// y offset between detector and clamp.

#ifdef DEBUG
	//showJ("CatchY:\t");
#endif // DEBUG		

	// Detect pen alone x-axis
	notDetected = true;
	pinMode(OPTIC_ENABLE_X_PIN, OUTPUT);
	digitalWrite(OPTIC_ENABLE_X_PIN, HIGH);
	pinMode(OPTIC_X_INPUT_PIN, INPUT);
	delay(1000);
	
	size_t x_max = 550 - abs(this->y) * 2.17f;		// Prevent too much x movement to y
	for (size_t i = 0; i < x_max; i++)
	{
		armGoLine((this->x + 0.2), this->y, this->z, step / 2);
#ifdef DEBUG
		//showJ();
#endif // DEBUG
		notDetected = digitalRead(OPTIC_X_INPUT_PIN);
		
		if (!notDetected)
		{
#ifdef DEBUG
			Serial.println("interruptX");
#endif // DEBUG			
			if (i > 300)
				armGoLine((this->x + offsetX), this->y, this->z + offsetZforfarX, step / 2);
			else
				armGoLine((this->x + offsetX), this->y, this->z, step / 2);
			break;
		}
		
	}

	digitalWrite(OPTIC_ENABLE_X_PIN, LOW);

#ifdef DEBUG
	Serial.println("X finished, lift up!!");
	//showJ("before lift: ");
	//showXYZ("be lifted:");
#endif // DEBUG	

	clawClamp(J, 'g');
	if (abs(this->y) > 68.5f)				// Prevent too much z movement to edged y
		liftPenHeight = 42;
	armGoLine(this->x, this->y, (this->z + liftPenHeight), step);

#ifdef DEBUG
	//Serial.println("Grabbed, go back!!");
	showXYZ("Grabbed, go back!!\n");
#endif // DEBUG	
	
	armGoDirect(initXYZ[0], initXYZ[1], initXYZ[2], angspeed);

	return 1;	// In case of slides or controller needs the return value.
}

/**------------------Drop Pen---------------------------------**/
int RoboticArmClass::DropPen(float canX, float canY, float canZ, float step, float angSpeed)
{

#ifdef DEBUG
	Serial.println("Start DropPen!!");
#endif // DEBUG

	armGoDirect(canX, canY, canZ, angSpeed);

#ifdef DEBUG
	Serial.println("Drop!!");
#endif // DEBUG	

	clawClamp(J, 'r');
	delay(1000);
	armGoDirect(initXYZ[0], initXYZ[1], initXYZ[2], angSpeed);

	return 1;
}

/**----------------------Writing-----------------------------**/

/*----------------------------Print and Show----------------------------------*/
void RoboticArmClass::showJ(const char * title)
{
	printOut(J, SIZEOF_ARRAY(J), title);
}

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

void RoboticArmClass::printOut(float * AR, size_t ARsize, const char * Hstring, const char * split)
{
	if (Hstring)
		Serial.print(Hstring);

	for (size_t i = 0; i < ARsize - 1; i++)
		Serial.print(String(AR[i], DegPrecision) + split + " ");
	Serial.print(String(AR[ARsize - 1], DegPrecision) + "\n");
}

void RoboticArmClass::printOut(float n, const char * Hstring, const char * endString)
{
	if (Hstring)
		Serial.print(String(Hstring) + ": ");
	Serial.print(String(n, DegPrecision) + endString);
}


RoboticArmClass Arm;