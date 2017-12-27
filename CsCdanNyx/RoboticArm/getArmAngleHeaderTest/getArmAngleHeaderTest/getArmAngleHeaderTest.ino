/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "Letters.h"
#include "RoboticArm.h"
//float step = 0.18f;				// Preferred: 0.25 with DegPrecision 3. Step speed for armGoLine.
//float angSpeed = 0.025f;		// Preferred: 1 with DegPrecision 3. Angular step for armGoDirect.
//
//float teststep = 2;
//float testang = 2;

void setup() {

	Serial.begin(9600);

	//Initial Position
	Arm.initServo();
	Arm.waitkey();
	Arm.initPosit(300, 0, 300);

	Arm.showJ("init J:\t");
	Arm.showXYZ("init Posit:\t");
	//Arm.waitkey();

	//Arm.armGoLine(327.125, -27.125, 311.334);
	//Arm.tiltAngle = 60 / Rad2Degree;
	//Arm.armGoDirect(300, 0, 300);
	//Arm.armGoLine(327.125, -27.125, 350);
	
	//// Letter.h test
	//Letters.initWord('T');
	//Letters.showLetterStrokes(4, true, "setup word:\n");
	//Letters.initLetter('T', M_PI_2 - 60 / Rad2Degree);

	//Letters.iterPrint();
	//Letters.initLetter('D', M_PI_2 - 60 / Rad2Degree);
	//Letters.showLetterStrokes(1, true, "After init:\n");



	//Arm.GrabPen(430, 0, 110);
	//Arm.waitkey();
	//Arm.DropPen(400, 50, 250);
	Arm.waitkey();

	Arm.setTilt(60);
	Arm.armGoDirect(300, 0, 300);
	Arm.showJ();

	//Arm.waitkey();
	//Arm.armGoLine(345, -75, 341.34);

	//Arm.chooseWord('T');
	//Arm.writeLetter('T');
	//Arm.writeLetter('D');
	//Arm.writeLetter('K');

	//Arm.chooseWord('N');
	//Arm.waitkey();
	//Arm.writeLetter('N');
	//Arm.waitkey();
	//Arm.writeLetter('F');
	//Arm.waitkey();
	//Arm.writeLetter('U');

	Arm.showJ("\nEnding J: ");
	Serial.println("End setup!!");
}

// the loop function runs over and over again until power down or reset
void loop() {

}