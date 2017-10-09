//
//
//

#include "RoboticArm.h"

///*----------------------------Arm's settings----------------------------------*/
//RoboticArmClass::RoboticArmClass()
//{
//
//}

/*--------------------------Initializations----------------------------------*/
void RoboticArmClass::initServo()
{
	Serial.println("//----Start Initiallization----//");

	servoAR[0].attach(servoPin0, 500, 2400);
	servoAR[1].attach(servoPin1, 500, 2400);
	servoAR[2].attach(servoPin2, 500, 2400);
	servoAR[3].attach(servoPin3, 500, 2400);
	servoAR[4].attach(servoPin4, 500, 2400);
	servoAR[5].attach(servoPin5, 500, 2400);

	servoAct();
}

void RoboticArmClass::initPosit(float ix, float iy, float iz, float angspeed)
{
	armGoDirect(ix, iy, iz, angspeed);
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
		servoAR[i].attach(i + 2, 500, 2400);
	}
}

void RoboticArmClass::servoDoJ()
{
	for (int i = 0; i < 6; i++) {
		servoAR[i].write(J[i]);
	}
#if SERVODELAY
	delay(SERVODELAY);
#endif // SERVODELAY
}

void RoboticArmClass::waitkey()
{
	while (Serial.read() != -1);
	while (Serial.read() == -1);
}

int RoboticArmClass::serialReadInt(bool needprint)
{
	//while (Serial.available())
	//	Serial.read();

	int incomingByte;
	int integerValue = 0;
	bool negsign = false;
	while (true)
	{
		incomingByte = Serial.read();
		if (incomingByte == '\n' || incomingByte == '\r' || incomingByte == 10 || incomingByte == 13 || incomingByte == 32)
			break;
		if (incomingByte == -1)
			continue;
		if (incomingByte == '-')
		{
			negsign = true;
			continue;
		}
		integerValue *= 10;
		integerValue = ((incomingByte - 48) + integerValue);
	}

	if (negsign) integerValue = -integerValue;

	if (needprint) Serial.println(integerValue);

	return integerValue;
}

void RoboticArmClass::servoAngTestByControl()
{
	while (true)
	{
		Serial.print("\nServo no (0~5): ");
		int servoNo = serialReadInt(true);

		if (servoNo == 72)		//'x' exit
		{
			Serial.println("Bye!!");
			break;
		}
		else if (servoNo >= 0 && servoNo < 6)
		{
			Serial.print("Relative Ang: ");
			int ang = baseDegree[servoNo] + serialReadInt(true);
			if (ang >= 0 && ang <= 180)
			{
				Serial.print(String("==> Servo [ ") + servoNo + " ] move [ " + ang + " ]\n");
				servoAR[servoNo].write(ang);
				J[servoNo] = ang;
			}
			else
			{
				Serial.println(String("Value excessed : ") + ang);
			}
		}
	}

}

void RoboticArmClass::readServoAng(float * Ang)
{
	for (size_t i = 0; i < 6; i++)
	{
		Ang[i] = servoAR[i].read();
	}
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
	/*check if r <= all arm*/
	//if (r < (arm[2] + arm[3])) r = 262;
	// Ang[2]
	Ang[2] = M_PI_2 - (acos((pow(r, 2) + pow(arm[2], 2) - pow(arm[3], 2)) / (2 * arm[2] * r))) - atan(rz / rx);
	// J[2] = pi/2 - ( The angle between r and arm[2] ) - ( The angle between r and XY plane )

	// Ang[3]
	// AsinIndCheck prevent asin from calculation over 1, which turns out to be -nan(ind), specified for yp == 65.
	float AsinIndCheck = (rx - arm[2] * sin(Ang[2])) / arm[3];

#ifdef ErrorOut
	if (AsinIndCheck >= 1)
	{
		Serial.println(String("AsinIndCheck error, when") + " x:" + xp + " y:" + yp + " z:" + zp + " AsinIn..k:" + String(AsinIndCheck, 8));
		AsinIndCheck = 0.99999;
	}
#endif // ErrorOut

	Ang[3] = -Ang[2] + M_PI_2 - asin(AsinIndCheck);

	// Ang[4]
	Ang[4] = -Ang[3] - Ang[2] + tiltAngle;

	// Rounding
	for (size_t i = 0; i < 5; i++)
		Ang[i] = round(Ang[i] * Rad2Degree * pow(10, DegPrecision)) / pow(10, DegPrecision);

	Ang[1] = -Ang[1];
	Ang[2] = -Ang[2];
	Ang[3] = Ang[3];


#ifdef ErrorOut
	if (isAngExcess(Ang))
	{
		Serial.print("Ang excessed, ");
		printOut(xp, "xp", ", ");
		printOut(yp, "yp", ", ");
		printOut(zp, "zp");
		printOut(Ang, 6, "Ang: ");
	}
#endif // ErrorOut

}

// Still in process
void RoboticArmClass::getArmPosition(float * Ang, float * XYZ)
{
}

bool RoboticArmClass::isAngExcess(float * Ang)
{
	for (size_t i = 0; i < 6; i++)
	{
		if ((baseDegree[i] + Ang[i]) > 180 || (baseDegree[i] + Ang[i]) < 0)
			return true;
	}

	return false;
}

/*-------------------------------Actions--------------------------------------*/
void RoboticArmClass::servoAct()
{
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
	getArmAngleDeg(xp, yp, zp, J);
	servoAct();

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
	float Ang[5];
	char sign[5] = { 1, 1, 1, 1, 1 };
	float tmp;

	getArmAngleDeg(xd, yd, zd, Ang);

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
		servoAct();
		for (size_t i = 0; i < 5; i++)
		{
			if (abs(Ang[i] - J[i]) <= angSpeed)
				sign[i] = 0;

			J[i] += sign[i] * angSpeed;
		}

#ifdef ErrorOut
		if (isAngExcess(J))
		{
			Serial.print("Ang excessed in GoDir, ");
			printOut(Ang, 6, "Ang: ");
		}
#endif // ErrorOut

		///print
#ifdef DEBUG
		//showJ();
		//showXYZ();
#endif // DEBUG

	}

	for (size_t i = 0; i < 5; i++)
		J[i] = Ang[i];

	servoAct();

	this->x = xd;
	this->y = yd;
	this->z = zd;

	///print
#ifdef DEBUG
	showJ("Final J: ");
	//showXYZ("Final XYZ: ");
#endif // DEBUG

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
	int initxyz[3] = { this->x,this->y,this->z };
	int liftPenHeight = 70;
	float offsetY = 10;
	float offsetX = 8;
	float GrabPtUpperBoundfromEnd = 5;
	bool notDetected = true;		// Check if optic detection detect pen. If detected, return false.
#ifdef DEBUG
	Serial.println("Release");
#endif // DEBUG
	clawClamp(J, 'r');

	// Initial position for optic detection
	/*
	armGoLine(this->x, this->y, this->z, step);
	armGoLine(this->x, this->y, penZ + 10, step);
	armGoLine(this->x, (this->y + 70.5), this->z, step);
	*/
	armGoDirect(this->x, (this->y + 70.5), penZ + 10, angspeed);
#ifdef DEBUG
	Serial.println("Posit ready for grab!!");
#endif // DEBUG
	// Detect pen alone y-axis
	pinMode(OPTIC_ENABLE_Y_PIN, OUTPUT);
	digitalWrite(OPTIC_ENABLE_Y_PIN, HIGH);
	pinMode(OPTIC_Y_INPUT_PIN, INPUT);
	delay(500);
	for (float i = 0; i <= 140; i += 0.5)
	{
		armGoLine(this->x, this->y - 0.5, this->z, step);
		notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
		if (!notDetected) {
			delay(1000);
			notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
			if (!notDetected)
			{
				//Serial.println("interruptY");
				break;
			}
		}
	}
	//digitalWrite(OPTIC_ENABLE_Y_PIN, LOW);
#ifdef DEBUG
	Serial.println("Y finished!!");
	
	//showJ("InY:\t");
#endif // DEBUG

	// Detect the end of the pen alone z-axis
	delay(500);
	for (size_t i = 0; i <= 200; i++)
	{
		armGoLine(this->x, this->y, this->z + 0.5, step / 2);
		notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
#ifdef DEBUG
		//Serial.println(notDetected?"not":"yes");
#endif // DEBUG			
		if (notDetected) {
			delay(1000);
			notDetected = digitalRead(OPTIC_Y_INPUT_PIN);
			if (notDetected)
			{
#ifdef DEBUG
				//Serial.println("End of the Pen at Z.");
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
	showJ("CatchZ:\t");
#endif // DEBUG	

	armGoLine(this->x, this->y + offsetY, this->z, step / 2);		// y offset between detector and clamp.

#ifdef DEBUG
	//showJ("CatchY:\t");
#endif // DEBUG		

	notDetected = true;
	// Detect pen alone x-axis
	pinMode(OPTIC_ENABLE_X_PIN, OUTPUT);
	digitalWrite(OPTIC_ENABLE_X_PIN, HIGH);
	pinMode(OPTIC_X_INPUT_PIN, INPUT);
	delay(1000);
	bool hasDetected = false;

	for (float i = 0; i <= 400; i++)
	{
		armGoLine((this->x + 0.2), this->y, this->z, step / 2);
#ifdef DEBUG
		//showJ();
#endif // DEBUG
		notDetected = digitalRead(OPTIC_X_INPUT_PIN);
		if (!notDetected)
		{
#ifdef DEBUG
			//Serial.println("interruptX");
#endif // DEBUG			
			
			armGoLine((this->x + offsetX), this->y, this->z, step / 2);
			break;
		}
		/*
		if (!notDetected)
		{
#ifdef DEBUG
			Serial.println("interruptX");
#endif // DEBUG
			
			hasDetected = true;
		}
		else if (hasDetected && notDetected)
		{
			break;
		}
		*/
	}
	digitalWrite(OPTIC_ENABLE_X_PIN, LOW);

#ifdef DEBUG
	Serial.println("X finished, lift up!!");
#endif // DEBUG	
	
	clawClamp(J, 'g');
	armGoLine(this->x, this->y, (this->z + liftPenHeight), step);

#ifdef DEBUG
	Serial.println("Grabbed, go back!!");
#endif // DEBUG	
	
	//armGoLine(initxyz[0], initxyz[1], initxyz[2], step);
	armGoDirect(initxyz[0], initxyz[1], initxyz[2], angspeed);
	return 1;	// In case of slides or controller needs the return value.
}

/**------------------Drop Pen---------------------------------**/
int RoboticArmClass::DropPen(float penX, float penY, float penZ, float step, float angspeed) {
	int initxyz[3] = { x,y,z };
	armGoLine((x + 50), y, z, step);
	armGoLine(x, y, penZ, step);
	armGoLine(penX, y, z, step);

#ifdef DEBUG
	Serial.println("Release");
#endif // DEBUG	

	clawClamp(J, 'r');
	armGoLine(initxyz[0], initxyz[1], initxyz[2], step);
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
//		Ang[i] = J[i] + baseDegree[i];
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