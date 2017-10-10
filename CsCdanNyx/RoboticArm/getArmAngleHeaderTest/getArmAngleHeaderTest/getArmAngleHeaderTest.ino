﻿/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board

#include "RoboticArm.h"
float step = 0.18f;				// Preferred: 0.25 with DegPrecision 3. Step speed for armGoLine.
float angSpeed = 0.025f;		// Preferred: 1 with DegPrecision 3. Angular step for armGoDirect.

float teststep = 2;
float testang = 2;

void setup() {

	Serial.begin(9600);

	/*
	//Initial Position
	Arm.initServo();
	Arm.waitkey();
	Arm.initPosit(300, 0, 300, angSpeed);

	Arm.showJ("init J:\t");
	Arm.showXYZ("init Posit:\t");

	//Arm.servoAngTestByControl();


	//Arm.waitkey();
	//Arm.GrabPen(430, 0, 110, step, angSpeed); //input final Destination for arm to Grab Pen
	
	Arm.waitkey();
	Arm.DropPen(400, 50, 250, step, angSpeed);


	Arm.showJ("\nEnding J: ");
	Serial.println("End setup!!");
	*/



	//----------------OnGoing-----------------------//
	Arm.initServo();

	Serial.read();
	Arm.initPosit(300, 0, 300, angSpeed);
	Serial.print("");

	Serial.read();
	Arm.GrabPen(430, 0, 110, step, angSpeed);
	Serial.print("");

	Serial.read();
	Arm.DropPen(400, 50, 250, step, angSpeed);
	Serial.print("");

}

// the loop function runs over and over again until power down or reset
void loop() {

}