#include "text.h"
#include "SerialClass.h"

#define CAMERA_0 0
#define CAMERA_1 1

int main()
{
	Task* task_list[3];
	task_list[0] = &(Task1{CAMERA_0});

	task_list[0]->execute();
}