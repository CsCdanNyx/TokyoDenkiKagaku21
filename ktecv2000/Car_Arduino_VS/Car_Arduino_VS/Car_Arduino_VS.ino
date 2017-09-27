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
	
	/*
	Serial.println("Distance? (cm)");
	Serial.flush();
	while (!Serial.available());
	//Serial.println("!");
	String tmp = Serial.readString();
	d = atoi(tmp.c_str());
	printf_serial("Distance = %d, Start!\n", d);
	*/
}

void loop()
{
	// reach check point

	// serial communication [board -> computer]
	// form computer task started

	// serial communication [computer -> board]
	// send data(distance) back to board
	Serial.flush();
	while (!Serial.available());
	delay(200);
	String serial_data = Serial.readString();
	printf_serial("=> %s\n=> At %d\n", serial_data.c_str(), serial_data.indexOf(","));

	// decode data
	int d_x, d_y;
	int pos = serial_data.indexOf(",");
	
	d_x = (int)serial_data.substring(0, pos).toFloat();
	d_y = (int)serial_data.substring(pos+1).toFloat();

	printf_serial("distance : (%d %d)\n", d_x, d_y);

	// move car
	car.wheel->move(d_x);
	// move slider
	
	car.slider->setDir(SLIDER_DIR_V);
	car.slider->move(d_y);
	
}
