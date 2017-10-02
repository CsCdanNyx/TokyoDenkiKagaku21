// 
// 
// 

#include "Letters.h"

void LettersClass::init(char TDKvNFU, float tiltang)
{


}

void LettersClass::setWord(const String& TDKvNFU)
{
	if (TDKvNFU == "TDK")
	{
		this->TDKorNFU = "TDK";	// Choose TDK
		
		// Letter T
		this->letterOne->letSize = 4;
		this->letterOne->letPts = new LetterPts[this->letterOne->letSize]{
			{ 'd',{  -10, -10,  -10 } },
			{ 'u',{  -10, 140,  -10 } },
			{ 'd',{   10,  65,   10 } },
			{ 'u',{ -155,  65, -155 } }
		};
		// Letter D
		this->letterTwo->letSize = 10;
		this->letterTwo->letPts = new LetterPts[this->letterTwo->letSize]{
			{ 'd',{   10,     10,   10 } },
			{ 'u',{ -155,     10, -155 } },
			{ 'd',{    0,      0,    0 } },
			{ 'd',{    0,      0,    0 } },
			{ 'd',{    0,      0,    0 } },
			{ 'd',{    0,      0,    0 } },
			{ 'd',{    0,      0,    0 } },
			{ 'd',{    0,      0,    0 } },
			{ 'd',{  -10, 46.54f,  -10 } },
			{ 'u',{  -10,    -10,  -10 } },
		};

		ADDpts(this->letterTwo->letPts[2].pts, this->letterTwo->letPts[3].pts,       0,     -84,       0);
		ADDpts(this->letterTwo->letPts[3].pts, this->letterTwo->letPts[4].pts, -17.17f, -25.15f, -17.17f);
		ADDpts(this->letterTwo->letPts[4].pts, this->letterTwo->letPts[5].pts, -29.98f, -13.09f, -29.98f);
		ADDpts(this->letterTwo->letPts[5].pts, this->letterTwo->letPts[6].pts,     -65,   6.34f,     -65);
		ADDpts(this->letterTwo->letPts[6].pts, this->letterTwo->letPts[7].pts, -30.51f,  35.03f, -30.51f);
		ADDpts(this->letterTwo->letPts[7].pts, this->letterTwo->letPts[8].pts,  -2.83f,  25.43f,  -2.83f);

		// Letter K


	}
	else if (TDKvNFU == "NFU")
	{
		this->TDKorNFU = "NFU";	// Choose NFU
		// Letter N

		// Letter F

		// Letter U

	}
	else
		Serial.println("??");
}

void LettersClass::initLetter(char clet, float tiltang, float letOriginPoint[3])
{
	currentLetter = getALetter(clet);
	for (size_t i = 0; i < currentLetter->letSize; i++)
	{
		currentLetter->letPts[i].pts[0] = letOriginPoint[0] + currentLetter->letPts[i].pts[0] * sin(tiltang);
		currentLetter->letPts[i].pts[1] = letOriginPoint[1] + currentLetter->letPts[i].pts[1];
		currentLetter->letPts[i].pts[2] = letOriginPoint[2] + currentLetter->letPts[i].pts[2] * cos(tiltang);
	}
	*currentLetter->iterPtr = 0;
}

void LettersClass::ADDpts(float despts[3], float srcpts[3], float px, float py, float pz)
{
	despts[0] = srcpts[0] + px;
	despts[1] = srcpts[1] + py;
	despts[2] = srcpts[2] + pz;
}

LettersClass::aLetter * LettersClass::getALetter(char clet)
{
	if (this->TDKorNFU == "TDK")
	{
		if (clet == 'T')		return this->letterOne;
		else if (clet == 'D')		return this->letterTwo;
		else if (clet == 'K')		return this->letterThree;
	}
	else if (this->TDKorNFU == "NFU")
	{
		if (clet == 'N')		return this->letterOne;
		else if (clet == 'F')		return this->letterTwo;
		else if (clet == 'U')		return this->letterThree;
	}
	Serial.println("Invalid letter, you should choose from letters " + this->TDKorNFU);
	return nullptr;
}

/*----------------------------Print and Show----------------------------------*/
void LettersClass::showLetterStrokes(bool printLift, const String& title)
{
	if (this->TDKorNFU == "TDK")
	{
		printOut(this->letterOne, true, title + "Letter T");
		printOut(this->letterTwo, true, title + "Letter D");
		//printOut(this->letterThree, true, title + "Letter K");
	}
	else if (this->TDKorNFU == "NFU")
	{
		Serial.println("NNNN");
		//printOut(this->letterOne, true, title + "Letter N");
		//printOut(this->letterTwo, true, title + "Letter F");
		//printOut(this->letterThree, true, title + "Letter U");
	}
	else
		Serial.println("???");
}

void LettersClass::printOut(aLetter * alet, bool printLift, const String& Hstring, const char * split)
{
	if (Hstring)
		Serial.println(String(Hstring) + ":");

	for (size_t i = 0; i < alet->letSize; i++)
	{
		LetterPts * &lpts = alet->letPts;
		if (printLift)
			Serial.print("[" + String(lpts[i].penlift) + "] ");
		Serial.print(String(lpts[i].pts[0], DegPrecision) + split + " ");
		Serial.print(String(lpts[i].pts[1], DegPrecision) + split + " ");
		Serial.print(String(lpts[i].pts[2], DegPrecision) + "\n");
	}
	Serial.println();
}

/*----------------------------Return Points-----------------------------------*/
float * LettersClass::getLetPts()
{
	return currentLetter->letPts[*currentLetter->iterPtr].pts;
}

char LettersClass::getLetLift()
{
	return currentLetter->letPts[*currentLetter->iterPtr].penlift;
}

bool LettersClass::nextPoint()
{
	if (++(*currentLetter->iterPtr) >= currentLetter->letSize)
		return false;
	currentLetter->iterPtr = nullptr;
	return true;
}

//void LettersClass::ptsAssign(float tmppts[3], float px, float py, float pz)
//{
//	tmppts[0] = px;
//	tmppts[1] = py;
//	tmppts[2] = pz;
//}

//LettersClass::LetterPts LettersClass::LetterPts::operator+(const float lpts[3]) const	// Overload operator+ to add two points
//{
//	LetterPts Let;
//	Let.penlift = this->penlift;
//	Let.pts[0] = this->pts[0] + lpts[0];
//	Let.pts[1] = this->pts[1] + lpts[1];
//	Let.pts[2] = this->pts[2] + lpts[2];
//	return Let;
//}


LettersClass Letters;
