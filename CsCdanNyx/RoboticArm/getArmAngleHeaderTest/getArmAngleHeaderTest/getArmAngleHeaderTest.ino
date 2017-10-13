/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board

#include "RoboticArm.h"


void setup() {

	Serial.begin(9600);

	//----------------OnGoing-----------------------//
	Arm.initServo();

	while(Serial.read() == -1);
	Arm.initPosit(300, 0, 300);
	Serial.println("Init Complete.");

	while (Serial.read() == -1);
	Arm.GrabPen(430, 0, 110);
	Serial.println("GrabPen Finished.");

	while (Serial.read() == -1);
	Arm.DropPen(400, 50, 250);
	Serial.println("DropPen Finished.");

	//Serial.println("End Challenge!!");
}

// the loop function runs over and over again until power down or reset
void loop() {

}