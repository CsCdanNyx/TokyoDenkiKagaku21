/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"

#include <inttypes.h>

float speed = 0.25;

//float initDeg[6] = { 90, 90, 150, 90, 150, 90 };

void setup() {

	Serial.begin(9600);

	//Initial Position
	Arm.init(220, 0, 360);

	//yp: -70~70 zp: 195~360
	//Arm.armGoLine(220, -70, 360, speed);
	//Arm.armGoLine(220,  70, 360, speed);
	//Arm.armGoLine(220,   0, 360, speed);
	//Arm.armGoLine(220,   0, 195, speed);
	//Arm.armGoLine(220,   0, 360, speed);

	//Arm.servoInit();

	Serial.flush();		// Flush Arduino Serial output.
	while (!Serial.available());	// Start looping after Serial port input.

}

// the loop function runs over and over again until power down or reset
void loop() {

	//Arm.setJ(initDeg);
	//Arm.servoDoJ();

	Arm.armGoLine(240, -80, 360, speed);
	Arm.armGoLine(240,  80, 360, speed);
	Arm.armGoLine(270,   0, 170, speed);
}