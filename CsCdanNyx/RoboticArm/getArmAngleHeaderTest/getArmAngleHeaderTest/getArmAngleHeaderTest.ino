/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"

#include "Servo.h"
#include <inttypes.h>


Servo servoAR[6];
float initDegree[] = { 90, 90, 90, 90, 150, 90 };

void setup() {

	for (int i = 0; i<6;) {
		servoAR[i].attach(++i, 500, 2400);
	}

	//Initial Position
	ServoAct(initDegree);
	Arm.init(300, 0, 270);

	for (size_t i = -70; i < 71; i++) {
		Arm.getArmAngleDeg(300, i, 270);
		float * degree = Arm.getJ();

		for (size_t i = 0; i < 6; i++) {
			degree[i] = initDegree[i] + degree[i];
		}
		ServoAct(degree);
	}

}

// the loop function runs over and over again until power down or reset
void loop() {
  
}

void ServoAct(float degree[]) {
	for (int i = 0; i<6;) {
		servoAR[i].write(degree[i++]);
	}
	delay(20);
}
