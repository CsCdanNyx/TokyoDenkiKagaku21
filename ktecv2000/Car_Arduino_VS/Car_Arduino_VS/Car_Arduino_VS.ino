#include "printf.h"
#include "car.h"
#include <Timer1\TimerOne.h>
#include "RoboticArm.h"
struct
{
	Wheel *wheel = nullptr;
	Slider *slider = nullptr;
} car;

PinSet pinSet;

void setup()
{
	// --------- BOARD SETUP ---------

#ifdef __WHEEL__
	// pin set
	pinSet.M1 = 12;
	pinSet.M2 = 13;
	pinSet.MOT_1 = 10;
	pinSet.MOT_2 = 11;
	pinSet.ON_CHECK = 2;
	pinSet.WHEEL_OI = 0;

	// pin mode
	pinMode(pinSet.MOT_1, OUTPUT);
	pinMode(pinSet.MOT_2, OUTPUT);
	pinMode(pinSet.M1, OUTPUT);
	pinMode(pinSet.M2, OUTPUT);
	//pinMode(pinSet.WHEEL_OI, INPUT);
	//pinMode(pinSet.ON_CHECK, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(pinSet.ON_CHECK), Car::setCheckPoint, LOW);

	// instance
	Wheel tmp_wheel(pinSet);
	car.wheel = &tmp_wheel;
	// car.wheel = &(Wheel)(pinSet);
#endif // __WHEEL__

#ifdef __SLIDER__
	// pin set
	pinSet.M1 = 12;
	pinSet.M2 = 13;
	pinSet.MOT_1 = 10;
	pinSet.MOT_2 = 11;

	// pin mode
	pinMode(pinSet.MOT_1, OUTPUT);
	pinMode(pinSet.MOT_2, OUTPUT);
	pinMode(pinSet.M1, OUTPUT);
	pinMode(pinSet.M2, OUTPUT);

	// instance
	Slider tmp_slider(pinSet);
	car.slider = &tmp_slider;
	// car.slider = &((Slider)(pinSet)); // wrong !

	// Arm setup
	Arm.initServo();
#endif // __SLIDER__

	// Color Sensor
#ifdef __COLOR_SENSOR__
	// pin set
	pinSet.S0 = 21;
	pinSet.S1 = 20;
	pinSet.S2 = 19;
	pinSet.S3 = 18;
	pinSet.LED = 16;
	pinSet.COLOR_OUT = 17;		// black wine
	pinSet.ON_CHECK = 8;

	// pin mode
	pinMode(pinSet.S0, OUTPUT);
	pinMode(pinSet.S1, OUTPUT);
	pinMode(pinSet.S2, OUTPUT);
	pinMode(pinSet.S3, OUTPUT);
	pinMode(pinSet.LED, OUTPUT);
	pinMode(pinSet.COLOR_OUT, INPUT);
	//pinMode(pinSet.ON_CHECK, OUTPUT);

	digitalWrite(pinSet.LED, HIGH);
	digitalWrite(pinSet.S0, HIGH);
	digitalWrite(pinSet.S1, LOW);

#endif // __COLOR_SENSOR__


	// ------ serial set ------
	Serial.begin(9600);
	Serial.flush();
}

// start
void task_0_start();
// detect target
void task_1_detect();
// knock-down
void task_2_recong();
// grab pen
void task_3_grab();
// drop pen
void task_4_drop();

// --- test code ---
void test_1()
{
	while (Serial.read() == -1);
	printf_serial("MOVE VERTICAL\n");
	Serial.flush();
	car.slider->setDir(SLIDER_DIR_V);
	car.slider->move(SLIDER_MOVE_T, 1);
	delay(1000);
	car.slider->move(SLIDER_MOVE_T, -1);

	while (Serial.read() == -1);
	printf_serial("MOVE HORIZONTAL\n");

	Serial.flush();
	car.slider->setDir(SLIDER_DIR_H);
	car.slider->move(SLIDER_MOVE_T, 1);
	delay(1000);
	car.slider->move(SLIDER_MOVE_T, -1);
}
void test_3_adjust_slider()
{
	pinMode(20, INPUT);
	char buf[6];
	while (Serial.available() <= 0);
	Serial.readBytes(buf, 5);
	delay(200);
	Serial.flush();
	if (buf[0] == 'V')
	{
		printf_serial("V ");
		car.slider->setDir(SLIDER_DIR_V);
	}
	else if (buf[0] == 'H')
	{
		printf_serial("H ");
		car.slider->setDir(SLIDER_DIR_H);
	}

	buf[5] = 0;
	uint8_t pwm = atoi(buf+2);
	printf_serial("%d ", pwm);
	if (buf[1] == '+')
	{
		printf_serial("+ \n");
		car.slider->move(SLIDER_MOVE_TEST, 1);
	}
	else if (buf[1] == '-')
	{
		printf_serial("- \n");
		car.slider->move(SLIDER_MOVE_TEST, -1, pwm);
	}
	Serial.flush();
	delay(300);
}
void test_4_read_sensor()
{
	while(1)
		car.wheel->read_sensor();
}
void test_5_line_follow_and_check_point()
{
	car.wheel->setCheckNum(1);
	car.wheel->move1();
}

// go, leave ch, stop
// go a little
// back, leave, touch
void loop()
{
	while (Serial.read() == -1);
	test_5_line_follow_and_check_point();
	while (1);
	//car.slider->setDir(SLIDER_DIR_V);
	//Serial.flush();
	//delay(100);
	//car.slider->move(SLIDER_DIR_V, 1);
	//while (1) car.wheel->read_sensor();
	//while (1)
	//{
	//	car.wheel->forward(255);
	//	isCheckPoint(pinSet);
	//}

	
	int check_num;
	{
		char buf[2];
		while (Serial.available() <= 0);
		Serial.readBytes(buf, 2);
		Serial.flush();
		check_num = atoi(buf);
	}
	
	car.wheel->setCheckNum(check_num);

	// ... adjust task execution by check_num
	switch (check_num)
	{
	case Car::CHECK_POINT_0_START:
	case Car::CHECK_POINT_1_PICTURE:
	case Car::CHECK_POINT_5_KEEP:
	case Car::CHECK_POINT_6_lCURVE:
	case Car::CHECK_POINT_7_rCURVE:
	case Car::CHECK_POINT_8_rCURVE:
	case Car::CHECK_POINT_12_rSPIN:
	case Car::CHECK_POINT_14_wPICTURE:
	case Car::CHECK_POINT_15_WRITE:
		task_0_start();
	case Car::CHECK_POINT_2_KEEP:
	case Car::CHECK_POINT_3_dPICTURE:
	case Car::CHECK_POINT_4_KNOCK:
		task_1_detect();
		task_2_recong();
	case Car::CHECK_POINT_13_PICK:
		task_3_grab();
		break;
	case Car::CHECK_POINT_16_DROP:
		task_4_drop();
	}

	while (1);

}

// start
void task_0_start()
{

#ifdef __WHEEL__
	while (Serial.read() == -1);
	Serial.flush();
	/// reach check point
	//car.wheel->halt();
	/// serial communication [board -> computer]
	/// form computer task started
	Serial.println("@ Start : button is pressed.");
	car.wheel->move1();
#endif // __WHEEL
	task_1_detect();
}
/// detect target
void task_1_detect()
{	
#ifdef __WHEEL__
	Serial.println("@ OK Wheel, had reached detect sign-stand check point");

	Serial.flush();
	delay(300);
	while (Serial.read() == -1);
	Serial.flush();
	delay(300);
	car.wheel->move1();

#endif // __WHEEL__
	task_2_recong();
}
/// knock-down
void task_2_recong()
{

#ifdef __WHEEL__

	//car.wheel->forward(255);
	Serial.println("@ OK Wheel, had reached knock-down check point");
#endif
	/// serial communication [computer -> board]
	/// send data(distance) back to board
	Serial.flush();
	delay(300);
	int pos;
	while ((pos = Serial.read()) == -1);
	/// decode data
	int pos_x = pos % 3, pos_y = pos / 3;
	/// move 

#ifdef __SLIDER__
	delay(5000);		// wait for wheel moving to specific position

	car.slider->setDir(SLIDER_DIR_V);
	car.slider->move(SLIDER_MOVE_T, car.slider->t_v_up * pos_y);
	delay(1000);
	car.slider->setDir(SLIDER_DIR_H);
	car.slider->move(SLIDER_MOVE_T, car.slider->t_h_pushOrPull);
	delay(1000);
	car.slider->setDir(SLIDER_DIR_H);
	car.slider->move(SLIDER_MOVE_T, -car.slider->t_h_pushOrPull);

	Serial.println("@ OK Slider, had successfully knock down sign-stand");

#endif // __SLIDER__

#ifdef __WHEEL
	if (pos_x == 0)
	{
		car.wheel->move1();
	}
	else if (pos_x == 1)
	{
		car.wheel->forward(200);
		delay(1000);
		car.wheel->backward(255);
		delay(30);
		car.wheel->halt();
	}
	else if (pos_x == 2)
	{
		car.wheel->move1();
	}
	
	Serial.flush();
	delay(300);
	while (Serial.read() == -1);
	car.wheel->move1();

#endif // __WHEEL

	task_3_grab();
}
/// pick pen
void task_3_grab()
{
#ifdef __WHEEL__
	Serial.println("@ OK Wheel, had reached pick-pen check point");
#endif
#ifdef __SLIDER__
	Serial.flush();
	delay(300);
	char cmd;
	while ((cmd = Serial.read()) == -1);
	if (cmd == 'S')
	{
		Serial.println("Pulling up slider to pick pen\n");

		delay(2000);
		car.slider->setDir(SLIDER_DIR_V);
		car.slider->move(SLIDER_MOVE_T, car.slider->t_v_pickPen);
		delay(2000);
		car.slider->setDir(SLIDER_DIR_H);
		car.slider->move(SLIDER_MOVE_T, car.slider->t_h_pickPen);

	}
	else
	{
		Serial.flush();
		delay(300);
	}

	while (Serial.read() == -1);
	Arm.initPosit(300, 0, 300);
	delay(1000);
	Arm.GrabPen(430, 0, 110);
	
	delay(2000);
	car.slider->move(SLIDER_MOVE_T, -(car.slider->t_h_pickPen));
	Serial.println("@ OK Arm, had successfully grab pen.");

#endif // __SLIDER__

#ifdef __WHEEL__
	Serial.flush();
	delay(300);
	while (Serial.read() == -1);
	car.wheel->move1();
#endif // __WHEEL__
	task_4_drop();
}
/// drop pen
void task_4_drop() 
{
#ifdef __WHEEL__
	delay(500);
	car.wheel->spin('R');
	delay(1200);
	car.wheel->brake();
	Serial.println("@ OK Wheel, had reached drop pen check point");

#endif

#ifdef __SLIDER__
	char cmd;
	while ((cmd = Serial.read()) == -1);
	if (cmd == 'S')
	{
		delay(2000);
		car.slider->setDir(SLIDER_DIR_V);
		car.slider->move(SLIDER_MOVE_T, car.slider->t_v_dropPen);
		delay(2000);
		car.slider->setDir(SLIDER_DIR_H);
		car.slider->move(SLIDER_MOVE_T, 0.5);
		Serial.println("@ OK Slider, push slider to drop pen position\n");

		delay(4000);
		car.slider->setDir(SLIDER_DIR_H);
		car.slider->move(SLIDER_MOVE_T, -0.5);
	}
	else
	{
		Serial.flush();
		delay(300);
	}

	while (Serial.read() == -1);

	Arm.DropPen(400, 50, 250);
	
#endif // __SLIDER__
}
