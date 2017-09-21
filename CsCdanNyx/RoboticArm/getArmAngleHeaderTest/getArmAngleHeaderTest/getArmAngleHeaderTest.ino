/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"

#include <inttypes.h>


Servo servoAR[6];
float initDegree[] = { 90, 90, 90, 90, 150, 90 };

void setup() {

	//Initial Position
	Arm.init(300, 0, 270);

	Arm.armGoLine(300, -70, 270, 1);
	Arm.armGoLine(300,  70, 270, 1);
	Arm.armGoLine(300,   0, 270, 1);
	Arm.armGoLine(300,   0, 105, 1);
	Arm.armGoLine(300,   0, 270, 1);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}