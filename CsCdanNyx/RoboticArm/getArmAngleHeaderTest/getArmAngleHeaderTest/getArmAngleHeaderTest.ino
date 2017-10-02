/*
 Name:		getArmAngleHeaderTest.ino
 Created:	2017/9/20 下午 02:27:14
 Author:	Danny
*/

// the setup function runs once when you press reset or power the board
#include "RoboticArm.h"

float speed = 0.25;		// Prefered: 0.25 with DegPrecision 3. Step speed for armGoLine.
float angSpeed = 1;		// Prefered: 1 with DegPrecision 3. Angular step for armGoDirect.

//float initDeg[6] = { 90, 90, 150, 90, 150, 90 };

void setup() {

	Serial.begin(9600);
	Arm.init(300 ,0, 220);
	//Initial Position

	//Arm.showJ("init J:\t");
	//Arm.showXYZ("init Position: ", true);

	//Arm.servoInit();

	//yp: -70~70 zp: 195~360
	//Arm.armGoLine(220, -70, 360, speed);
	//Arm.armGoLine(220,  70, 360, speed);
	//Arm.armGoLine(220,   0, 360, speed);
	//Arm.armGoLine(220,   0, 195, speed);
	//Arm.armGoLine(220,   0, 360, speed);

	//Arm.armGoDirect(250, 0, 300, angSpeed);
	//Serial.println("Revert!!");
	//Arm.armGoDirect(300, 0, 300, angSpeed);

	Serial.flush();					// Flush Arduino Serial output.
	while (!Serial.available());	// Start looping after Serial port input.

	//float * Ang = Arm.getJ();
	//Ang[4] = 70;
	//Arm.setJ(Ang);
	//Arm.servoAct();

	// *Reached z point = PenCapHeight 50 + *( PenGrabHeight 70 + penZ )
	
	//Arm.armGoLine(380, 0, 280, speed);

	//float ang[6];
	//Arm.getArmAngleDeg(380, 0, 280, ang);
	//Arm.printOut(ang, SIZEOF_ARRAY(ang), "desti ang:  ");

	Serial.println("End!!");
}

// the loop function runs over and over again until power down or reset
void loop() {

	//Arm.setJ(initDeg);
	//Arm.servoDoJ();

	Arm.armGoLine(300 -70, 220, speed);
	Arm.armGoLine(300,  70, 220, speed);
	Arm.armGoLine(300 ,  0, 200 ,speed);
	
	//Arm.armGoDirect(120, 0, 360, angSpeed);
	//Arm.armGoDirect(220, 0, 360, angSpeed);
}