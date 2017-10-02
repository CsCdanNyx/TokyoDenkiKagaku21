/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "Letters.h"
#include "RoboticArm.h"



void setup() {

	Serial.begin(9600);

	// Initial Position
	Arm.init(300, 0, 300);
	Arm.showJ("init J:\t");
	Arm.showXYZ("init Position: ", true);


	//// Letter.h test
	Letters.setWord("TDK");
	Letters.showLetterStrokes(true, "HAHAHA\t");
	float initPoint[3] = { 10, 10, 10 };
	Letters.initLetter('T', 30, initPoint);
	Letters.initLetter('D', 30, initPoint);
	Letters.showLetterStrokes();

	Serial.println("1st End!!");

	// Fool-proofing segment
	Serial.flush();					// Flush Arduino Serial output.
	while (!Serial.available());	// Start looping after Serial port input.
	
	//Arm.init(300, 0, 300);
	
	//Arm.GrabPen(400, 0, 150);

	Serial.println("setup End!!");

}

// the loop function runs over and over again until power down or reset
void loop() {



}