/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"

#include <inttypes.h>


Servo servoAR[6];

void setup() {
	Serial.begin(9600);

	//Initial Position
	//Arm.init(220, 0, 360);

	//yp: -70~70 zp: 195~360
	//Arm.armGoLine(220, -70, 360, 1);
	//Arm.armGoLine(220,  70, 360, 1);
	//Arm.armGoLine(220,   0, 360, 1);
	//Arm.armGoLine(220,   0, 195, 1);
	//Arm.armGoLine(220,   0, 360, 1);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}