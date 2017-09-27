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
	char target;
	std::cout << "Target?\n";
	std::cin >> target;
	
	setTarget(target);

	cv::VideoCapture video(getCamera());
	if (!video.isOpened()) {
		std::cout << "Camera error\n";
		return;
	}
	Sleep(500);
	cv::Mat pic;
	video >> pic;
	//std::cout << "Pic size : " << target.pic.size() << "  " << target.pic.cols << "," << target.pic.rows << "\n";
	text_recong(pic);
	//std::cout << "center coordinate of digit : " << target.center.x << "," << target.center.y << "\n";
	Object object = getObject();

	float prop_x = digit_2_w / float(object.bound.width);
	float prop_y = digit_2_h / float(object.bound.height);

	std::cout << "propotion : " << prop_x << "," << prop_y << "\n";

	float arm_x_dis = (object.center.x - (pic.cols / 2)) * prop_x * 100;
	float arm_y_dis = (object.center.y - (pic.rows / 2)) * prop_y * -1 * 100;
	std::cout << "@ the distance from center : " << arm_x_dis << "," << arm_y_dis << "\n";
	std::string arm_xy_serial = std::to_string(prop_x).substr(0, 6) + "," + std::to_string(prop_y).substr(0, 6);

	cv::waitKey(0);
	
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
	
}