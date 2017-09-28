/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"

float speed = 0.25;		// Prefered: 0.25 with DegPrecision 3. Step speed for armGoLine.
float angSpeed = 0.5;		// Prefered: 1 with DegPrecision 3. Angular step for armGoDirect.

//float initDeg[6] = { 90, 90, 150, 90, 150, 90 };

void setup() {

	Serial.begin(9600);

	// Initial Position
	Arm.init(300, 0, 300);

	Arm.showJ("init J:\t");
	Arm.showXYZ("init Position: ", true);

	// Fool-proofing segment
	Serial.flush();					// Flush Arduino Serial output.
	while (!Serial.available());	// Start looping after Serial port input.
	
	Arm.init(300, 0, 300);
	
	Arm.GrabPen(400, 0, 150);

	Serial.println("End!!");

}

// the loop function runs over and over again until power down or reset
void loop() {



}