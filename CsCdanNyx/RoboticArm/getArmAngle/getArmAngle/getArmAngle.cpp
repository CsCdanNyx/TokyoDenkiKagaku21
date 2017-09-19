// getArmAngle.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

#define Rad2Degree 180/M_PI
float * getArmAngleRad(float xp, float yp, float zp, int RadPrecision = 0);
float * getArmAngleDeg(float J[6], float xp, float yp, float zp, int DegPrecision = 0);

int main()
{	
	float xp = 300, yp = 0, zp = 270;

	/*
	yp = 65;
	float degree[6] = { 0 };
	getArmAngleDeg(degree, xp, yp, zp, 0);
	for (size_t i = 0; i < 6; i++)
		cout << degree[i] << " ";
	*/

	/*
	FILE * fp;
	fopen_s(&fp, "../../../TempData/ttest2.txt", "w");
	float degree[6] = { 0 };
	getArmAngleDeg(degree, xp, yp, zp, 2);
	
	cout << "\ndegree:\n";
	for (size_t i = 0; i < 6; i++)
		cout << degree[i] << " ";
	cout << endl;

	for (size_t i = 0; i < 6; i++)
		fprintf(fp, "%f,", degree[i]);
	fprintf(fp, "\n");
	fclose(fp);
	*/

	/*
	ofstream f;
	f.open("../../../TempData/ttest3.txt");
	float degree[6] = { 0 };
	getArmAngleDeg(degree, xp, yp, zp, 0);
	for (size_t i = 0; i < 6; i++)
		f << degree[i] << ',';
	f << endl;
	f.close();
	*/


	ofstream f;
	f.open("../../../TempData/ttest1.txt");
	float degree[6] = { 0 };
	for (yp = -70; yp < 71; yp++)
	{
		getArmAngleDeg(degree, xp, yp, zp, 0);
		for (size_t i = 0; i < 6; i++)
			f << degree[i] << ',';
		f << endl;
	}

	f << "*********************" << endl;
	
	yp = 0;
	for (float i = zp; i > zp-166; i--)
	{	
		getArmAngleDeg(degree, xp, yp, i, 0);
		for (size_t i = 0; i < 6; i++)
			f << degree[i] << ',';
		f << endl;
	}

	f.close();


    return 0;
}

float * getArmAngleRad(float xp, float yp, float zp, int RadPrecision)
{
	float deltay = 4.5, deltaz = 0;				//Servos' coordinate compensations.
	float arm[4] = { 99, 159, 104.5, 93.595 };	//Arms' length.
	float J[6] = { 0 };
	bool parallel = true;						//Paralllel
	float rx = 0, rz = 0;
	/*	Begin calculations.  */
	J[0] = asin((deltay + yp) / arm[0]);
	J[1] = -J[0];

	if (parallel)
	{
		deltaz = 134.2 - 20;
		rx = xp - arm[3] - arm[0] * cos(J[1]);
		rz = zp - deltaz;
	}
	else
		;
	
	float r = sqrt( pow(rx, 2) + pow(rz, 2) );
	float theta = atan( rz / rx );
	float a1 = r * cos(theta);
	float b1 = acos((pow(r, 2) + pow(arm[1], 2) - pow(arm[2], 2)) / (2 * arm[1]*r) );

	J[2] =  M_PI_2 - b1 - theta;
	J[3] = J[2] + M_PI_2 - asin((a1 - 159 * sin(J[2])) / arm[2]);

	if (parallel)
		J[4] = J[3] - J[2];
	else
		J[4] = J[3] - J[2] + M_PI/3;

	for (size_t i = 0; i < 6; i++)
		J[i] = round(J[i] * pow(10, RadPrecision)) / pow(10, RadPrecision);

	return J;
}

float * getArmAngleDeg(float J[6], float xp, float yp, float zp, int DegPrecision)
{
	float deltay = 4.5, deltaz = 0;				//Servos' coordinate compensations.
	float arm[4] = { 99, 159, 104.5, 93.595 };	//Arms' length.
//	float J[6] = { 0 };
	///
	for (size_t i = 0; i < 6; i++)
		J[i] = 0;

	bool parallel = true;						//Paralllel
	float rx = 0, rz = 0;
	/*	Begin calculations.  */
	J[0] = asin((deltay + yp) / arm[0]);
	J[1] = -J[0];

	if (parallel)
	{
		deltaz = 134.2 - 20;
		rx = xp - arm[3] - arm[0] * cos(J[1]);
		rz = zp - deltaz;
	}
	else
		;

	float r = sqrt(pow(rx, 2) + pow(rz, 2));
	float theta = atan(rz / rx);
	float a1 = r * cos(theta);
	float b1 = acos((pow(r, 2) + pow(arm[1], 2) - pow(arm[2], 2)) / (2 * arm[1] * r));



	J[2] = M_PI_2 - b1 - theta; 
	
	float AsinCheck = (a1 - 159 * sin(J[2])) / arm[2];	
	if (AsinCheck >= 1 && AsinCheck < 1.05)		// Set for hit yp == 65 AsinCheck will be 1, which asin would return "-nan(ind)".
		AsinCheck = 0.99999;

	J[3] = J[2] + M_PI_2 - asin(AsinCheck);


	if (parallel)
		J[4] = J[3] - J[2];
	else
		J[4] = J[3] - J[2] + M_PI / 3;

	for (size_t i = 0; i < 6; i++)
		J[i] = round(J[i] * Rad2Degree * pow(10, DegPrecision)) / pow(10, DegPrecision);

	return J;
}
