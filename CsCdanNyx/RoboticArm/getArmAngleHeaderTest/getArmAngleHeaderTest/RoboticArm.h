// RoboticArm.h

#ifndef _ROBOTICARM_h
#define _ROBOTICARM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Enable debugging
#define DEBUG
#define ErrorOut
//#define ErrorHandle

// Pin settings
//// Servo's Pin settings
#define servoPin0	2
#define servoPin1	3
#define servoPin2	4
#define servoPin3	5
#define servoPin4	6
#define servoPin5	7

//// Interrupt Pin
#define OPTIC_Y_INPUT_PIN  8
#define OPTIC_X_INPUT_PIN  9
#define OPTIC_ENABLE_Y_PIN 15
#define OPTIC_ENABLE_X_PIN 14

// Some parameters could be set
#define STEPSPEED		0.18f
#define ANGULARSPEED	0.025f
#define DegPrecision	3			// Prefered: 3 with speed 0.25, angSpeed 1. Angle's decimal precision.
#define SERVODELAY		0			// Prefered: 0 by reason of servos' vibration (Due to intterupt triggering). Function servoAct's delaying.


const float CM2UNIT = 10;					// Defines how many coordinate units in 1 cm (1unit ~= 1mm).
// Pen
const float PenGrabHeight = 7 * CM2UNIT;

// For calculation.
#define _USE_MATH_DEFINES
#include <math.h>
const float Rad2Degree = 180 / M_PI;
// For printing
#define SIZEOF_ARRAY(array) (sizeof(array)/sizeof(*array))

#include "Servo.h"
#include <TimerOne.h>

class RoboticArmClass
{
public:
	/*--------------------------Initializations----------------------------------*/
	//RoboticArmClass();
	void initServo();
	void initPosit(float ix = 300, float iy = 0, float iz = 300, float angSpeed = ANGULARSPEED);
	
	int ArmErrorHandle();
	///// Debugging ///////////////////////////
	void setJ(float * Ang);
	void servoInit();
	void servoDoJ();

	void waitkey();
	int serialReadInt(bool needprint = false);	// Read in an int or -int.
	void servoAngTestByControl();				// Directly type in angle to control servo.
	void readServoAng(float * Ang);
	///////////////////////////////////////////

	/*----------------------Angle & Path Calculations----------------------------*/
	void getArmAngleDeg(float xp, float yp, float zp, float * Deg, bool setError = false);

	void getArmPosition(float * Ang, float * XYZ);

	bool isAngExcess(float * Ang, bool setError = false);
	

	//void adjPosition();				// Adjust position x, y, z value from Servos' angle J.
	//bool isAngleFailed();				// Check if Servos' angle hit the limitations.

	//void Jzero();

	/*-------------------------------Actions--------------------------------------*/
	void servoAct(bool ERRcheck = false);													// Servos' signal output.
	/**-----------------------Arm--------------------------------**/
	void armGoTo(float xp, float yp, float zp);							// Arm move to point.
	void armGoLine(float xd, float yd, float zd, float step = STEPSPEED);			// Move to destination linearly.
	void armGoDirect(float xd, float yd, float zd, float angSpeed = ANGULARSPEED);		// Move to destination directly and angularly by changing angle per angSpeed degree.
	/**-----------------------Claw--------------------------------**/
	void clawClamp(float * Ang, char RelvClp);		// Release or Clamp the clamp. RelvClp: 'r' for release, 'c' for clamp.

	/*-------------------------------Challenge--------------------------------------*/
	/**------------------Grab Marker Pen-------------------------**/
	int GrabPen(float penX, float penY, float penZ, float step = STEPSPEED, float angSpeed = ANGULARSPEED);

	/**------------------Drop pen---------------------------------**/
	int DropPen(float canX, float canY, float canZ, float step = STEPSPEED, float angSpeed = ANGULARSPEED);
	/**----------------------Writing-----------------------------**/
	void LiftPen(float * Ang, char UpvDn, float penliftAng = 20);			// Lift up or down the pen for the next stroke. UpvDn: 'u' for up, 'd' for down.

	/*----------------------------Print and Show----------------------------------*/
	void showJ(const char * title = NULL);
	//void showAbJ(const char * title = NULL);
	void showXYZ(const char * title = NULL, bool XYZdisp = false);
	void printOut(float * AR, size_t ARsize, const char * Hstring = NULL, const char * split = ",");	//Print out array.
	void printOut(float n, const char * Hstring = NULL, const char * endString = "\n");					//Print out variable.

	float * getJ();
	float * getXYZ();
	//void moveArmPath(float xd, float yd, float zd, float step = 1);	// step defines the distance(cm) arm moves in 1 step.
	float J[6] = { -75, 0, 90, 0, 40, 60 };				// Each Servo's angle.   Rest arm angle: absolute(J): 12(-75),93(0),180(90),90(0),180(40).

private:
	float baseDegree[6] = { 90, 90, 90, 90, 140, 60 };	// Base Angle for calculations.
	float x = 0, y = 0, z = 0;							// Position coordinate.
	//float J[6] = { -75, 0, 90, 0, 40, 60 };				// Each Servo's angle.   Rest arm angle: absolute(J): 12(-75),93(0),180(90),90(0),180(40).
	//float J[6] = { 0, 0, 90, 0, 40, 0 };
	float tiltAngle = 0;			// alpha			// The angle of inclination of the plane which the Claw parallels to.
	Servo servoAR[6];

	uint8_t ERRcode = 0;			// 0 success, -1 restoration, -2 asin err, 1 ang excessed ( > 0 could effect on arm execution, while < 0 not. )
	// Arm's constant settings(mm).
	const float arm[5] = { 99, 134.2f, 159, 104.5f, 93.9875f };	// Arms' length.
	//		    arm[5]: J0-J1, J1-J2, J2-J3, J3-J4, J4-XYZ.
	const float delYc = 4.5f;				// deltay			// The distance between J1 and Y on y-axis.
	const float arm4ToXYang = -0.214436f;	// thetap			// The angle between arm[4] and X-Y parallel plane.
};

extern RoboticArmClass Arm;

#endif
