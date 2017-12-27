// Letters.h

#ifndef _LETTERS_h
#define _LETTERS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// Enable debugging
#define DEBUG

/// Duplicated defines needed configurations.
/// 
#define DegPrecision	3			// Preferred: 3 with speed 0.25, angSpeed 1. Angle's decimal precision.
#define MM2UNIT			0.8f

#define _USE_MATH_DEFINES
#include <math.h>
const float Degree2Rad = M_PI / 180;


class LettersClass
{
 protected:
	 struct LetterPts
	 {
		 char penlift;
		 float pts[3];
		 //LetterPts operator+(const float lpts[3]) const;		// Overload operator+ to add two points
	 };

	 struct aLetter
	 {
		 //char clet;
		 float LetOrigin[3];		// Letter origin on arm's coordinate.
		 size_t letSize;
		 LetterPts * letPts = nullptr;
		 size_t * iterPtr = nullptr;
	 };

 public:

	void setWord(char TDKvNFU);
	void initLetter(char alet);
	void initLetter(int letNo);
	void tiltLetter(float tiltangInRad);
	void deleteLetter();
	//friend class RoboticArmClass;

	/*----------------------------Return Points-----------------------------------*/
	float * getLetPts();	// Get current Letter's iterating point
	char getLetLift();		// Get current Letter's iterating point's penlift
	bool nextPoint();		// Iterate to current Letter's next point. If next null, return false.

	/*----------------------------Print and Show----------------------------------*/
	void showLetterStrokes(uint8_t No = 4, bool printLift = true, const String& title = String());
	void printOut(aLetter * alet, bool printLift = true, const String& Hstring = String(), const char * split = ",");
	
	void iterPrint();
	//void ADDpts(float despts[3], float srcpts[3], float px, float py, float pz);

 private:
	 char TDKorNFU;

	 aLetter * letterOne   = new aLetter;
	 aLetter * letterTwo   = new aLetter;
	 aLetter * letterThree = new aLetter;
	 
	 aLetter * currentLetter = nullptr;

	 //void ptsAssign(float tmppts[3], float px, float py, float pz);
	 void ADDpts(uint8_t despts, uint8_t srcpts /*, float px, float py, float pz*/);
	
	 aLetter * getALetter(char clet);
};

extern LettersClass Letters;

#endif

