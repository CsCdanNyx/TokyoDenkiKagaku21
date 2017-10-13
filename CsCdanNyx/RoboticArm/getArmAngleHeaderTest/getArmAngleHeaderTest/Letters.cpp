// 
// 
// 

#include "Letters.h"


void LettersClass::setWord(char TDKvNFU)
{
	if (TDKvNFU == 'T')
	{
		this->TDKorNFU = 'T';	// Choose TDK
	}
	else if (TDKvNFU == 'N')
	{
		this->TDKorNFU = 'N';	// Choose NFU
	}
	else
		Serial.println("??");
}

void LettersClass::initLetter(char alet)
{
	if (this->TDKorNFU == 'T')
	{
		if (alet == 'T')			// Letter T
		{
			this->currentLetter = new aLetter;
			currentLetter->iterPtr = new size_t();
			currentLetter->LetOrigin[0] = 380;
			currentLetter->LetOrigin[1] = -65;
			currentLetter->LetOrigin[2] = 340;

			currentLetter->letSize = 4;
			currentLetter->letPts = new LetterPts[currentLetter->letSize]{
				{ 'd',{  -10, -10,  -10 } },
				{ 'u',{  -10, 140,  -10 } },
				{ 'd',{   10,  65,   10 } },
				{ 'u',{ -155,  65, -155 } }
			};

		}
		else if (alet == 'D')		// Letter D
		{
			this->currentLetter = new aLetter;
			currentLetter->iterPtr = new size_t();
			currentLetter->LetOrigin[0] = 380;
			currentLetter->LetOrigin[1] = -65;
			currentLetter->LetOrigin[2] = 340;

			currentLetter->letSize = 10;
			currentLetter->letPts = new LetterPts[currentLetter->letSize]{
				{ 'd',{      10,      10,      10 } },
				{ 'u',{    -155,      10,    -155 } },
				{ 'd',{       0,     -84,       0 } },		//0
				{ 'd',{ -17.17f, -25.15f, -17.17f } },		//0
				{ 'd',{ -29.98f, -13.09f, -29.98f } },		//0
				{ 'd',{     -65,   6.34f,     -65 } },		//0
				{ 'd',{ -30.51f,  35.03f, -30.51f } },		//0
				{ 'd',{  -2.83f,  25.43f,  -2.83f } },		//0
				{ 'd',{    -10,   46.54f,     -10 } },
				{ 'u',{    -10,      -10,     -10 } },
			};

			ADDpts(7, 8);
			ADDpts(6, 7);
			ADDpts(5, 6);
			ADDpts(4, 5);
			ADDpts(3, 4);
			ADDpts(2, 3);

			//ADDpts(2, 3, 0, -84, 0);
			//ADDpts(3, 4, -17.17f, -25.15f, -17.17f);
			//ADDpts(4, 5, -29.98f, -13.09f, -29.98f);
			//ADDpts(5, 6, -65, 6.34f, -65);
			//ADDpts(6, 7, -30.51f, 35.03f, -30.51f);
			//ADDpts(7, 8, -2.83f, 25.43f, -2.83f);

		}
		else if (alet == 'K')		// Letter K
		{
			this->currentLetter = new aLetter;
			currentLetter->iterPtr = new size_t();
			currentLetter->LetOrigin[0] = 0;
			currentLetter->LetOrigin[1] = 0;
			currentLetter->LetOrigin[2] = 0;

			currentLetter->letSize = 6;
			currentLetter->letPts = new LetterPts[currentLetter->letSize]{
				{ 'd',{   10,  10,   10 } },
				{ 'u',{ -185,   0, -185 } },
				{ 'd',{   10, 128,   10 } },
				{ 'u',{ -125, -10, -125 } },
				{ 'd',{  -52,  40,  -52 } },
				{ 'u',{ -176, 130, -176 } }
			};

			ADDpts(1, 0);
		}

	}
	else if (this->TDKorNFU == 'N')
	{
		if (alet == 'N')			// Letter N
		{
			this->currentLetter = new aLetter;
			currentLetter->iterPtr = new size_t();
			currentLetter->LetOrigin[0] = 0;
			currentLetter->LetOrigin[1] = 0;
			currentLetter->LetOrigin[2] = 0;

			currentLetter->letSize = 6;
			currentLetter->letPts = new LetterPts[currentLetter->letSize]{
				{ 'd',{   10,  10,   10 } },
				{ 'u',{ -185,   0, -185 } },
				{ 'd',{   14,   0,   14 } },
				{ 'u',{ -175, 126, -175 } },
				{ 'd',{   10, 119,   10 } },
				{ 'u',{ -185,   0, -185 } }
			};

			ADDpts(1, 0);
			ADDpts(5, 4);
		}
		else if (alet == 'F')		// Letter F
		{
			this->currentLetter = new aLetter;
			currentLetter->iterPtr = new size_t();
			currentLetter->LetOrigin[0] = 0;
			currentLetter->LetOrigin[1] = 0;
			currentLetter->LetOrigin[2] = 0;

			currentLetter->letSize = 6;
			currentLetter->letPts = new LetterPts[currentLetter->letSize]{
				{ 'd',{   10,  10,   10 } },
				{ 'u',{ -185,   0, -185 } },
				{ 'd',{  -10, -10,  -10 } },
				{ 'u',{  -10, 120,  -10 } },
				{ 'd',{  -80, -10,  -80 } },
				{ 'u',{  -80, 110,  -80 } }
			};

			ADDpts(1, 0);
		}
		else if (alet == 'U')		// Letter U
		{

		}

	}
#ifdef DEBUG
	else
		Serial.println("??");
#endif // DEBUG
}

void LettersClass::tiltLetter(float tiltangInRad)
{
	for (size_t i = 0; i < currentLetter->letSize; i++)
	{
		currentLetter->letPts[i].pts[0] = currentLetter->LetOrigin[0] + currentLetter->letPts[i].pts[0] * sin(tiltangInRad);
		currentLetter->letPts[i].pts[1] = currentLetter->LetOrigin[1] + currentLetter->letPts[i].pts[1];
		currentLetter->letPts[i].pts[2] = currentLetter->LetOrigin[2] + currentLetter->letPts[i].pts[2] * cos(tiltangInRad);
	}
}

void LettersClass::deleteLetter()
{
	if (currentLetter)
	{
		if (currentLetter->iterPtr)
		{
			delete currentLetter->iterPtr;
			currentLetter->iterPtr = nullptr;
#ifdef DEBUG
			Serial.println("Iter deleted!!");
#endif // DEBUG
		}
		if (currentLetter->letPts)
		{
			delete[] currentLetter->letPts;
			currentLetter->letPts = nullptr;
#ifdef DEBUG
			Serial.println("Pts deleted!!");
#endif // DEBUG
		}
		delete currentLetter;
#ifdef DEBUG
		Serial.println("Letter deleted!!");
#endif // DEBUG
	}
}

void LettersClass::ADDpts(uint8_t despts, uint8_t srcpts /*, float px, float py, float pz*/)
{
	currentLetter->letPts[despts].pts[0] += currentLetter->letPts[srcpts].pts[0];
	currentLetter->letPts[despts].pts[1] += currentLetter->letPts[srcpts].pts[1];
	currentLetter->letPts[despts].pts[2] += currentLetter->letPts[srcpts].pts[2];

	//currentLetter->letPts[despts].pts[0] = currentLetter->letPts[srcpts].pts[0] + px;
	//currentLetter->letPts[despts].pts[1] = currentLetter->letPts[srcpts].pts[1] + py;
	//currentLetter->letPts[despts].pts[2] = currentLetter->letPts[srcpts].pts[2] + pz;
}

LettersClass::aLetter * LettersClass::getALetter(char clet)
{
	if (this->TDKorNFU == 'T')
	{
		if (clet == 'T')		return this->letterOne;
		else if (clet == 'D')		return this->letterTwo;
		else if (clet == 'K')		return this->letterThree;
	}
	else if (this->TDKorNFU == 'N')
	{
		if (clet == 'N')		return this->letterOne;
		else if (clet == 'F')		return this->letterTwo;
		else if (clet == 'U')		return this->letterThree;
	}
	Serial.println("\nInvalid letter, you should choose from letters " + this->TDKorNFU);
	return nullptr;
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
	{
		delete currentLetter->iterPtr;
		currentLetter->iterPtr = nullptr;
#ifdef DEBUG
		Serial.println("Iter deleted in nxt!!");
#endif // DEBUG
		return false;
	}

	return true;
}

/*----------------------------Print and Show----------------------------------*/
void LettersClass::showLetterStrokes(uint8_t No, bool printLift, const String& title)
{
	if (this->TDKorNFU == 'T')
	{
		if (No == 1 || No == 4)
			printOut(this->letterOne, true, title + "Letter T");
		else if (No == 2 || No == 4)
			printOut(this->letterTwo, true, title + "Letter D");
		else if (No == 3 || No == 4)
			printOut(this->letterThree, true, title + "Letter K");
	}
	else if (this->TDKorNFU == 'N')
	{
		Serial.println("NNNN");
		if (No == 1 || No == 4)
			printOut(this->letterOne, true, title + "Letter N");
		else if (No == 2 || No == 4)
			printOut(this->letterTwo, true, title + "Letter F");
		else if (No == 3 || No == 4)
			printOut(this->letterThree, true, title + "Letter U");
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

void LettersClass::iterPrint()
{
	//Serial.println(String(clet) + "\'s Pts:");
	do
	{
		float * tmppts;
		tmppts = getLetPts();
		Serial.print(String(tmppts[0], 3) + ", " + String(tmppts[1], 3) + ", " + String(tmppts[2], 3) + "\n");
	} while (nextPoint());
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
