#include "printf.h"
#include "car.h"
#include <Timer1\TimerOne.h>


struct
{
	Wheel *wheel;
	Slider *slider;
} car;


volatile long int t = 0;
int d = 0;
int over = 0;

#ifdef __COLOR_SENSOR__
void color_sensor_set(PinSet &pin)
{
	pinMode(pin.S0, OUTPUT);
	pinMode(pin.S1, OUTPUT);
	pinMode(pin.S2, OUTPUT);
	pinMode(pin.S3, OUTPUT);
	pinMode(pin.COLOR_OUT, INPUT);
	digitalWrite(pin.S0, HIGH);
	digitalWrite(pin.S1, HIGH);
}
#endif
#ifdef __WHEEL__
void Wheel::on_check_point()
{
	// codes for reaching check point
}
#endif
void setup()
{
	PinSet pinSet;
	// ------ pin initialize ------
	// PWM
	pinSet.M1 = 12;
	pinSet.M2 = 13;
	pinSet.MOT_1 = 10;
	pinSet.MOT_2 = 11;
	pinSet.ON_CHECK = 9;

#ifdef __WHEEL__
	pinSet.WHEEL_OI = 0;

	car.wheel = &(Wheel)(pinSet);
#endif // __WHEEL__

	// Color Sensor
#ifdef __COLOR_SENSOR__
	pinSet.S0 = 6;
	pinSet.S1 = 7;
	pinSet.S2 = 8;
	pinSet.S3 = 9;
	pinSet.COLOR_OUT = 3;

	car.slider = &(Slider)(pinSet);
#endif // __COLOR_SENSOR__

	// ------ serial set ------
	Serial.begin(9600);
	
	Serial.println("Distance? (cm)");
	Serial.flush();
	while (!Serial.available());
	//Serial.println("!");
	/*
	String tmp = Serial.readString();
	d = atoi(tmp.c_str());
	printf_serial("Distance = %d, Start!\n", d);
	*/
}

void loop()
{
	
	/*
	printf_serial("Over\n");
	while (1)
		car.halt();
	*/
}
