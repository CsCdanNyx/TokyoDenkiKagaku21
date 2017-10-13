#include <stdio.h>
#include <tchar.h>
#include <string>

#include "SerialClass.h"	
#include "text.h"

#define digit_2_w 10.6
#define digit_2_h 16.1

//int _tmain(int argc, _TCHAR* argv[])
Task1::Task1(int camera) : Task(camera)
{

}
void Task1::execute()
{
	/// target detection
	std::cout << "Camera => " << getCamera() << "\n";
	cv::VideoCapture video(getCamera());
	if (!video.isOpened()) {
		std::cout << "Camera error\n";
		return;
	}
	Sleep(500);
	cv::Mat pic;
	video >> pic;
	if (getTarget())
	{
		text_recong(pic);
	}
	else
	{
		while(!getTarget())
			text_recong(pic);
		return;
	}
	
	/// calculate moving distance
	/*
	Object object = getObject();

	float prop_x = digit_2_w / float(object.bound.width);
	float prop_y = digit_2_h / float(object.bound.height);

	std::cout << "propotion : " << prop_x << "," << prop_y << "\n";

	int arm_x_dis = (int)((object.center.x - (pic.cols / 2)) * prop_x * 100);
	int arm_y_dis = (int)((object.center.y - (pic.rows / 2)) * prop_y * -1 * 100);
	std::cout << "@ the distance from center : " << arm_x_dis << "," << arm_y_dis << "\n";
	std::string arm_xy_serial = std::to_string(prop_x) + "," + std::to_string(prop_y);

	cv::waitKey(0);
	
	/// communicate with boards
	std::cout << "--- Start serial communication ---\n"
			  << "Please give the device port\n";
	char dev[10];
	std::cin >> dev;

	Serial* SP = new Serial("COM8");    // adjust as needed

	if (SP->IsConnected())
		std::cout << "Connected\n";

	char incomingData[256] = "";
	int dataLength = 255;
	int len_read = 0;

	while (! SP->WriteData( (TCHAR*) arm_xy_serial.c_str() , arm_xy_serial.size()));

	std::cout << "Serial write success!\n";

	Sleep(500);
	*/	
}