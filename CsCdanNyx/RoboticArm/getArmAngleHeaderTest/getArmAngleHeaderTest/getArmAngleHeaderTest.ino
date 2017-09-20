/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"
void setup() {
	Arm.init(300, -70, 270);
	Arm.moveArmPath(300, 70, 270, 1);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
