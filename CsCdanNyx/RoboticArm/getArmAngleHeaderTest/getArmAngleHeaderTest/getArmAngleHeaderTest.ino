﻿/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"
float speed = 0.15f;		// Prefered: 0.25 with DegPrecision 3. Step speed for armGoLine.
float angSpeed = 0.025f;		// Prefered: 1 with DegPrecision 3. Angular step for armGoDirect.
//float initDeg[6] = { 90, 90, 150, 90, 150, 90 };
void setup() {

	Serial.begin(9600);
	Arm.init(300 ,0, 300);
	Arm.showJ("init J:\t");
	Arm.showXYZ("init Posit:\t");

	Serial.flush();					// Flush Arduino Serial output.
	while (!Serial.available());	// Start looping after Serial port input.
	//Arm.armGoLine(301, 1, 300.5, speed);
	Arm.GrabPen(430, 0, 110, speed); //input final Destination for arm to Grab Pen
	

}

// the loop function runs over and over again until power down or reset
void loop() {

}
