/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board

#include "RoboticArm.h"
float step = 0.18f;				// Prefered: 0.25 with DegPrecision 3. Step speed for armGoLine.
float angSpeed = 0.025f;		// Prefered: 1 with DegPrecision 3. Angular step for armGoDirect.

float teststep = 2;
float testang = 2;

void setup() {

	Serial.begin(9600);
	
	//Initial Position
	Arm.initServo();
	Arm.waitkey();
	Arm.initPosit(300, 0, 300, angSpeed);

	Arm.showJ("init J:\t");
	Arm.showXYZ("init Posit:\t");

	//Arm.servoAngTestByControl();

	//yp: -70~70 zp: 195~360
	//Arm.armGoLine(220, -70, 360, speed);
	//Arm.armGoLine(220,  70, 360, speed);
	//Arm.armGoLine(220,   0, 360, speed);
	//Arm.armGoLine(220,   0, 195, speed);
	//Arm.armGoLine(220,   0, 360, speed);

	//Arm.armGoDirect(250, 0, 300, angSpeed);
	//Serial.println("Revert!!");
	//Arm.armGoDirect(300, 0, 300, angSpeed);

	//while (!Serial.available());	// Start looping after Serial port input.

	Arm.waitkey();

	//Arm.armGoLine(300, -104, 260, teststep);
	Arm.GrabPen(430, 0, 200, step, angSpeed); //input final Destination for arm to Grab Pen
	//Arm.DropPen(400, 0,110, speed);


	Serial.println("End setup!!");
}

// the loop function runs over and over again until power down or reset
void loop() {
	//Arm.setJ(initDeg);
	//Arm.servoDoJ();

	//Arm.armGoLine(240, -70, 360, speed);
	//Arm.armGoLine(240,  70, 360, speed);
	//Arm.armGoLine(270,   0, 200, speed);

	//Arm.armGoDirect(120, 0, 360, angSpeed);
	//Arm.armGoDirect(220, 0, 360, angSpeed);


	//Serial.println("Enter Servo no.(0~5) and relative deg :");
	//int incomingByte = 0;
	////while (Serial.available()) {

	//Serial.read();
	//while (true)
	//{
	//	incomingByte = Serial.read();
	//	if (incomingByte == '\r' || incomingByte == '\n')
	//	{
	//		continue;
	//	}
	//	else if (incomingByte == 'x')
	//	{
	//		Serial.println("Bye!!");
	//		//Serial.flush();
	//		break;
	//	}
	//	else if (incomingByte == -1)
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		Serial.print("I received: ");
	//		Serial.println(incomingByte);
	//		break;
	//	}

	//}


	//int servoNo;
	//float ang = 0;
	//Serial.println("Enter Servo no.(0~5) and relative deg :");
	//if (Serial.available() > 0)
	//{
	//	servoNo = Serial.read();
	//	/*if (servoNo == -1)
	//	{
	//		continue;
	//	}*/
	//	/*else if (servoNo == 'x')
	//	{
	//		Serial.println("Bye!!");
	//		break;
	//	}*/
	//	if (servoNo > 5 || servoNo < 0)
	//	{
	//		Serial.println("\nPlease enter 0~5.");
	//		//continue;
	//	}
	//	else
	//	{
	//		float ag = Serial.read();
	//		Serial.print("servoNo:");
	//		Serial.print(servoNo);
	//		Serial.print("  ag:");
	//		Serial.println(ag);
	//	}
	//}
}