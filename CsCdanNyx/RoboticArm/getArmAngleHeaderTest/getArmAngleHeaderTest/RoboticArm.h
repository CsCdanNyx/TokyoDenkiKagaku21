// RoboticArm.h

#ifndef _ROBOTICARM_h
#define _ROBOTICARM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define CM2UNIT	10					// Defines how many coordinate units in 1 cm (1unit ~= 1mm).
// Pen
#define PenGrabHeight	6 * CM2UNIT	

// Arm's properties.

// Servo's Pin settings
#define servoPin0	2
#define servoPin1	3
#define servoPin2	4
#define servoPin3	5
#define servoPin4	6
#define servoPin5	7

// Some parameters could be set
#define STEPSPEED		0.25		// Prefered: 0.25 with DegPrecision 3. Step speed for armGoLine.
#define ANGULARSPEED	0.5			// Prefered: 1 with DegPrecision 3. Angular step for armGoDirect.
#define DegPrecision	3			// Prefered: 3 with speed 0.25, angSpeed 1. Angle's decimal precision.
#define DELAY	0					// Prefered: 0 by reason of servos' vibration (Due to intterupt triggering). Function servoAct's delaying.

// For printing
#define SIZEOF_ARRAY(array) (sizeof(array)/sizeof(*array))

#define _USE_MATH_DEFINES
#include <math.h>
#include "Servo.h"
#include "Letters.h"

// For calculation.
const float Rad2Degree = 180 / M_PI;

float initPoint[3] = {300, 0, 300};

class RoboticArmClass
{
 public:
	/*--------------------------Initializations----------------------------------*/
	//RoboticArmClass();
	void init(float ix = 300, float iy = 0, float iz = 300);
	
	///// Debugging ///////////////////////////
	void setJ(float * Ang);
	void servoInit();
	void servoDoJ();
	///////////////////////////////////////////

	/*----------------------Angle & Path Calculations----------------------------*/
	void getArmAngleDeg(float xp, float yp, float zp, float * Ang);

	void getArmPosition(float * Ang, float * XYZ);


	//void adjPosition();				// Adjust position x, y, z value from Servos' angle J.
	//bool isAngleFailed();				// Check if Servos' angle hit the limitations.
	
	//void Jzero();

	/*-------------------------------Actions--------------------------------------*/
	void servoAct();													// Servos' signal output.
	/**-----------------------Arm--------------------------------**/
	void armGoTo(float xp, float yp, float zp);							// Arm move to point.
	void armGoLine(float xd, float yd, float zd, float step = STEPSPEED);			// Move to destination linearly.
	void armGoDirect(float xd, float yd, float zd, float angSpeed = ANGULARSPEED);		// Move to destination directly and angularly by changing angle per angSpeed degree.
	/***--------------------Overload---------------------***/
	void armGoLine(float desXYZ[3], float step = STEPSPEED);			// Move to destination linearly.
	void armGoDirect(float desXYZ[3], float angSpeed = ANGULARSPEED);					// Move to destination directly and angularly by changing angle per angSpeed degree.
																						/**-----------------------Claw--------------------------------**/
	void clawClamp(float * Ang, char RelvClp);		// Release or Clamp the clamp. RelvClp: 'r' for release, 'c' for clamp.

	/*----------------------------Print and Show----------------------------------*/
	void showJ(const char * title = NULL);
	//void showAbJ(const char * title = NULL);
	void showXYZ(const char * title = NULL, bool XYZdisp = false);
	void printOut(float * AR, size_t ARsize, const char * Hstring = NULL, const char * split = ",");	//Print out array.
	void printOut(float n, const char * Hstring = NULL, const char * endString = "\n");					//Print out variable.

	float * getJ();
	float * getXYZ();
	//void moveArmPath(float xd, float yd, float zd, float step = 1);	// step defines the distance(cm) arm moves in 1 step.


	/*-------------------------------Challenge--------------------------------------*/
	/**------------------Grab Marker Pen-------------------------**/
	int GrabPen(float penX, float penY, float penZ);

	/**----------------------Writing-----------------------------**/
	void setPenLift(float * Ang, char UpvDn, float penAng);			// Lift up or down the pen for the next stroke. UpvDn: 'u' for up, 'd' for down.
	void chooseWord(const String& TDKorNFU);
	void writeLetter(char clet, float LetOrigin[3], float tilt);


 private:

	float initDegree[6] = { 90, 90, 90, 90, 150 };	// Servos' initial degree.
	float x = initPoint[0], y = initPoint[1], z = initPoint[2];						// Position coordinate.
	float J[6] = { 0, 0, 0, 0, 0, 0 };				// Each Servo's angle.
	//bool parallelToFloor = true;					// Parallel to the ground, otherwise it would parallel to the whiteboard.
	float tiltAngle = 0 / Rad2Degree;			// alpha		// The angle of inclination of the plane which the Claw parallels to.
	Servo servoAR[6];

	
	// Arm's constant settings(mm).
	const float arm[5] = { 99, 134.2f, 159, 104.5f, 93.9875f };	// Arms' length.
	//		    arm[5]: J0-J1, J1-J2, J2-J3, J3-J4, J4-XYZ.
	const float delYc = 4.5f;				// deltay			// The distance between J1 and Y on y-axis. 
	const float arm4ToXYang = -0.214436f;	// thetap			// The angle between arm[4] and X-Y parallel plane.

	//-------Challenge settings
	bool needPenlift = false;
	float penliftAng = 20 / Rad2Degree;
};

extern RoboticArmClass Arm;

#endif

