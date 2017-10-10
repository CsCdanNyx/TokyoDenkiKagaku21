#include "printf.h"
#include "car.h"

#include <Timer1\TimerOne.h>

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

	//printf_serial("Start!\n");
}

char task_1_target;
int task_2_pos;
// start
void task_0();
// detect target
void task_1();
// knock-down
void task_2();
void keyes_ir();

void is_stop();

void loop1()
{
	while (Serial.read() == -1);
	car.wheel->move(WHEEL_MOVE_GO);
	
	//car.wheel->left(1);
	//while(1) car.wheel->read_sensor();
	printf_serial("over\n");
	delay(200);
}

void loop()
{
	Serial.flush();

	while (Serial.read() == -1);
	car.wheel->move(WHEEL_MOVE_GO);

	Car::clearCheckPoint();
	task_0();
	Car::clearCheckPoint();
	task_1();
	Car::clearCheckPoint();
	task_2();
	Car::clearCheckPoint();
	while (1);

}

// start
void task_0()
{

#ifdef __WHEEL__
	while (Serial.read() == -1);
	Serial.flush();
	/// reach check point
	//car.wheel->halt();
	/// serial communication [board -> computer]
	/// form computer task started
	Serial.println("Button is pressed, start moving");
	car.wheel->move(WHEEL_MOVE_GO);
#endif // __WHEEL

}
// detect target
void task_1()
{	
#ifdef __WHEEL__
	Serial.println("reaching Sign-stand check point");

	Serial.flush();
	delay(300);
	while (Serial.read() != -1);
	while (Serial.read() == -1);
	Serial.flush();
	delay(300);
	car.wheel->move(WHEEL_MOVE_GO);

#endif // __WHEEL__

}
// knock-down
void task_2()
{

#ifdef __WHEEL__

	//car.wheel->forward(255);
	Serial.println("Reaching knock-down check point");
#endif
	/// serial communication [computer -> board]
	/// send data(distance) back to board
	Serial.flush();
	delay(300);
	int pos;
	while ((pos = Serial.read()) == -1);
	/// decode data
	Serial.println(pos, DEC);//////
	int pos_x = pos % 3, pos_y = pos / 3;
	//printf_serial("%d %d\n", d_x, d_y);/////////
	/// move 

#ifdef __SLIDER__
	delay(5000);
	car.slider->setDir(SLIDER_DIR_V);
	car.slider->move(SLIDER_MOVE_T, car.slider->t_v_up * pos_y);
	delay(1000);
	car.slider->setDir(SLIDER_DIR_H);
	car.slider->move(SLIDER_MOVE_T, car.slider->t_h_pushOrPull);
	delay(1000);
	car.slider->setDir(SLIDER_DIR_H);
	car.slider->move(SLIDER_MOVE_T, -car.slider->t_h_pushOrPull);

#endif // __SLIDER__

#ifdef __WHEEL
	if (pos_x == 0)
	{
		car.wheel->move(WHEEL_MOVE_GO, 1);
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
		car.wheel->move(WHEEL_MOVE_BACK, 1);
	}
#endif // __WHEEL


}

void keyes_ir()
{
	int enablePin = 4;
	pinMode(enablePin, OUTPUT);
	int outputPin = 3;
	pinMode(outputPin, INPUT);
	bool objectDetect = false;
	while (1)
	{
		digitalWrite(enablePin, HIGH);     // Enable the internal 38kHz signal.
		delayMicroseconds(210);                   // Wait 210?s (8 pulses of 38kHz).
		if (digitalRead(outputPin))        // If detector Output is HIGH,
		{
			objectDetect = false;           // then no object was detected;
		}
		else                                // but if the Output is LOW,
		{
			delayMicroseconds(395);               // wait for another 15 pulses.
			if (digitalRead(outputPin))    // If the Output is now HIGH,
			{                               // then first Read was noise
				objectDetect = false;       // and no object was detected;
			}
			else                            // but if the Output is still LOW,
			{
				objectDetect = true;        // then an object was truly detected.
			}
		}
		digitalWrite(enablePin, LOW);      // Disable the internal 38kHz signal.
	}
}

void is_stop()
{
	car.slider->stop = true;
}