/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"

float speed = 1.2f;		// Prefered: 0.25 with DegPrecision 3. Step speed for armGoLine.
float angSpeed = 0.25f;		// Prefered: 1 with DegPrecision 3. Angular step for armGoDirect.

//float initDeg[6] = { 90, 90, 150, 90, 150, 90 };

void setup() {

	Serial.begin(9600);
	Arm.init(280 ,0, 300);
	Arm.showJ("init J:\t");
	Arm.showXYZ("init Posit:\t");

	Serial.flush();					// Flush Arduino Serial output.
	while (!Serial.available());	// Start looping after Serial port input.


	/*Arm.armGoLine(350,  0,   250, speed);
	Arm.armGoLine(400, 0, 220, speed);*/

	Arm.armGoLine(340, 0, 300, speed);
	Arm.armGoLine(340, 0, 110, speed);
	Arm.armGoLine(430, 0, 110, speed);
	

	/*float ang[6];
	Arm.getArmAngleDeg(370, 0, 240, ang);
	Arm.printOut(ang, SIZEOF_ARRAY(ang), "desti ang:  ");*/

	Arm.showJ();
	Serial.println("End!!");
}

// the loop function runs over and over again until power down or reset
void loop() {

	//Arm.setJ(initDeg);
	//Arm.servoDoJ();

	//Arm.armGoLine(300 -70, 220, speed);
	//Arm.armGoLine(300,  70, 220, speed);
	//Arm.armGoLine(300 ,  0, 200 ,speed);
	
	//Arm.armGoDirect(120, 0, 360, angSpeed);
	//Arm.armGoDirect(220, 0, 360, angSpeed);
}