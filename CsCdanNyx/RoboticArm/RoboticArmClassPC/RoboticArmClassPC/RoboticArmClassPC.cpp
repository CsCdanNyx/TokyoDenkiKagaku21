// RoboticArmClassPC.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "RoboticArm.h"

//Arduino incapible
#include <string>

int main()
{
	Arm.init(300, 0, 270);
	//Arm.getArmAngleDeg(300, 70, 270);
	//Arm.showJ();
	//Arm.showPosition();
	Arm.moveArmPathPrint(300, 0, 105, 1, "../../../TempData/RbArmtest2.txt");
    return 0;
}

