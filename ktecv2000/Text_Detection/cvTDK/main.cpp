#include "text.h"
#include "SerialClass.h"

#define CAMERA_0 0
#define CAMERA_1 2

int main()
{
	cv::VideoCapture video(0);
	Task1 t(0);
	while (1)
	{
		cv::Mat pic;
		video >> pic;
		Sleep(100);
		video >> pic;
		t.text_recong(pic);
	}
	//text_train();
}

int main1()
{

	Task* task_list[3];
	Task1 task1(CAMERA_0);
	task_list[0] = &task1;
	//task_list[0] = &(Task1{CAMERA_0});

	{
		/*
		Serial* SP1 = new Serial("COM8");
		if (SP1->IsConnected())
			std::cout << "(Leonardo) Connected\n";
		else
		{
			std::cout << "Device(Leonardo) not founded\n";
			return -1;
		}
		std::cout << "Press to write data...\n";
		//std::cin.ignore(INT_MAX);
		int go;
		std::cin >> go;

		char wbuf[] = "wdata";
		while (!SP1->WriteData((TCHAR*)wbuf, 6))
		{
			std::cout << ".";
			Sleep(300);
		}
		std::cout << "(Leonardo)Serial write success!\n";

		char buf[50];
		while (!SP1->ReadData(buf, 10))
		{
			Sleep(500);
			std::cout << ".";
		}
		std::cout << "\n\nSerial read success!\n" << " Data=> " << buf << "\n";

		return 0;
		*/
	}

	//task_list[0]->execute();
	Serial* SP1 = new Serial("COM6");    // adjust as needed
	//Serial* SP2 = new Serial("COM11");

	if (SP1->IsConnected())
		std::cout << " - COM6(Leonardo) Connected\n";
	
	else
	{
		std::cout << " - Device(Leonardo) not founded\n";
		return -1;
	}
	/*
	if (SP2->IsConnected())
		std::cout << "COM11(UNO) Connected\n";
	
	else
	{
		std::cout << "Device(UNO) not founded\n";
		return -1;
	}
	
	*/
	/// wait for sign-detect task
	char buf[200];
	while (!SP1->ReadData(buf, 200))
	{
		Sleep(500);
		std::cout << ".";
	}
	{
		int i = 0;
		while (buf[i++] != '\n');
		buf[i] = 0;
	}
	std::cout << "\n[*] ----- Start movong! ------\n";
	std::cout << "[*] Receive instruction from board : " << buf << "\n";
	std::cin.sync();
	Sleep(500);

	std::cout << "[*] Wait for board ready\n";
	while (!SP1->ReadData(buf, 200))
	{
		Sleep(500);
		std::cout << ".";
	}
	{
		int i = 0;
		while (buf[i++] != '\n');
		buf[i] = 0;
	}
	std::cout << "\n\n[*] ----- Arrive at Task_1 (Sign detect) -----\n" << "[*] Receive instruction from board : " << buf << "\n";
	std::cin.sync();
	Sleep(1000);
	
	task_list[0]->execute();
	std::cout << "\n[*] Task_1 is done !\n";

	char target = task_list[0]->getTarget();
	char wbuf[] = { target , '\0'};
	while (!SP1->WriteData((TCHAR*)wbuf, 1))
	{
		std::cout << ".";
		Sleep(300);
	}
	std::cout << "[*] Serial write success!\n";
	/*
	while (!SP2->WriteData((TCHAR*)wbuf, 6))
	{
		std::cout << ".";
		Sleep(300);
	}
	std::cout << "(UNO)Serial write success!\n";
	*/
	
	std::cin.sync();
	Sleep(500);
	/// wait for knock-down task
	std::cout << "[*] Wait for board ready\n";
	while (!SP1->ReadData(buf, 200))
	{
		Sleep(500);
		std::cout << ".";
	}
	{
		int i = 0;
		while (buf[i++] != '\n');
		buf[i] = 0;
	}
	std::cout << "\n\n[*] ----- Arrive at Task_2 (Knock Down) -----\n" << "[*] Receive instruction from board : " << buf << "\n";
	std::cin.sync();
	Sleep(1000);

	task_list[0]->execute();

	std::cout << "\n[*] Task_2 is done !\n";

	int pos = task_list[0]->getObject().pos;
	char wbuf[] = { pos , '\0' };

	//char wbuf[] = "30,30";
	while (!SP1->WriteData((TCHAR*)wbuf, 1))
	{
		std::cout << ".";
		Sleep(300);
	}
	std::cout << "[*] Serial write success!\n";

	/*
	while (!SP2->WriteData((TCHAR*)wbuf, 6))
	{
	std::cout << ".";
	Sleep(300);
	}
	std::cout << "(UNO)Serial write success!\n";
	*/

	std::cout << "[*] Wait for board ready\n";

}