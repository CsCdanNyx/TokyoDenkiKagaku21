/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "Letters.h"
#include "RoboticArm.h"

float step = 0.18f;				// Preferred: 0.25 with DegPrecision 3. Step speed for armGoLine.
float angSpeed = 0.025f;		// Preferred: 1 with DegPrecision 3. Angular step for armGoDirect.


void setup() {

	Serial.begin(9600);

	//Initial Position
	Arm.initServo();
	Arm.waitkey();
	Arm.initPosit(300, 0, 300);

	Arm.showJ("init J:\t");
	Arm.showXYZ("init Posit:\t");
	Arm.waitkey();

	//// Letter.h test
	Letters.setWord("TDK");
	Letters.showLetterStrokes(true, "HAHAHA\t");
	float initPoint[3] = { 10, 10, 10 };
	Letters.initLetter('T', 30, initPoint);
	Letters.initLetter('D', 30, initPoint);
	Letters.showLetterStrokes();



	Arm.showJ("\nEnding J: ");
	Serial.println("End setup!!");
}

// the loop function runs over and over again until power down or reset
void loop() {

}