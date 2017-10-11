// Letters.h

#ifndef _LETTERS_h
#define _LETTERS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/// Duplicated defines needed configurations.
#define DegPrecision	3			// Preferred: 3 with speed 0.25, angSpeed 1. Angle's decimal precision.

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
		 size_t letSize;
		 LetterPts * letPts = nullptr;
		 size_t * iterPtr = nullptr;
	 };

 public:

	void init(char TDKvNFU, float tiltang);

	void setWord(const String& TDKvNFU);
	void initLetter(char alet, float tiltang, float letOriginPoint[3]);

	//friend class RoboticArmClass;

	/*----------------------------Print and Show----------------------------------*/
	void showLetterStrokes(bool printLift = true, const String& title = String());
	void printOut(aLetter * alet, bool printLift = true, const String& Hstring = String(), const char * split = ",");
	
	//void ADDpts(float despts[3], float srcpts[3], float px, float py, float pz);

	/*----------------------------Return Points-----------------------------------*/
	float * getLetPts();	// Get current Letter's iterating point
	char getLetLift();		// Get current Letter's iterating point's penlift
	bool nextPoint();		// Iterate to current Letter's next point. If next null, return false.

 private:
	 String TDKorNFU;

	 aLetter * letterOne   = new aLetter;
	 aLetter * letterTwo   = new aLetter;
	 aLetter * letterThree = new aLetter;
	 
	 aLetter * currentLetter = nullptr;

	 //void ptsAssign(float tmppts[3], float px, float py, float pz);
	 void ADDpts(float despts[3], float srcpts[3], float px, float py, float pz);
	
	 aLetter * getALetter(char clet);
};

extern LettersClass Letters;

#endif

