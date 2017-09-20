#include <stdio.h>
#include <tchar.h>
#include <string>

#include "SerialClass.h"	
#include "text.h"

#define digit_2_w 10.6
#define digit_2_h 16.1

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	if (argc < 2)
	{
		std::cout << "arg1 : device\n";
		return -1;
	}*/

	Target target;
	//std::cin >> target.digit;
	target.digit = 2;
	cv::VideoCapture video(1);
	if (!video.isOpened()) {
		std::cout << "Camera error\n";
		return -1;
	}
	Sleep(500);
	video >> target.pic;
	std::cout << "Pic size : " << target.pic.size() << "  " << target.pic.cols << "," << target.pic.rows << "\n";
	text_recong(target);
	std::cout << "center coordinate of digit : " << target.center.x << "," << target.center.y << "\n";

	float prop_x = digit_2_w / float(target.bound.width);
	float prop_y = digit_2_h / float(target.bound.height);

	std::cout << "propotion : " << prop_x << "," << prop_y << "\n";

	float arm_x_dis = (target.center.x - (target.pic.cols / 2)) * prop_x ;
	float arm_y_dis = (target.center.y - (target.pic.rows / 2)) * prop_y * -1;
	std::cout << "@ the distance from center : " << arm_x_dis << "," << arm_y_dis << "\n";
	cv::waitKey(0);
	/*
	std::string arm_xy_serial = std::to_string(prop_x) + "," + std::to_string(prop_y);

	Serial* SP = new Serial(argv[1]);    // adjust as needed

	if (SP->IsConnected())
		std::cout << "Connected\n";

	char incomingData[256] = "";
	int dataLength = 255;
	int len_read = 0;

	//while (SP->IsConnected())
	{
		len_read = SP->ReadData(incomingData, dataLength);
		printf("Bytes read: (0 means no data available) %i\n", len_read);
		incomingData[len_read] = 0;

		printf("=> %s\n", incomingData);

		Sleep(500);
	}*/
	return 0;
}