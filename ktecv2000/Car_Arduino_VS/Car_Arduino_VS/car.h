// car.h

#ifndef _CAR_h
#define _CAR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define __COLOR_SENSOR__


static void gear();
static void time_tick();

typedef struct PinSet
{
	uint8_t M1, M2;
	uint8_t MOT_1, MOT_2;

#ifdef __COLOR_SENSOR__
	uint8_t S0, S1, S2, S3, COLOR_OUT;
#endif // __COLOR_SENSOR__

	// ----------------
#ifdef __WHEEL__
	uint8_t WHEEL_OI;
	uint8_t PITCH;
	uint8_t TEETH;
#endif // __WHEEL__

	// ----------------
	uint8_t ON_CHECK;
};

class Car
{
public:
	Car(PinSet &pin);
	//virtual void move(int d) = 0;

protected:
	virtual void move(int d) = 0;
	virtual void forward() = 0;
	virtual void backward() = 0;
	void halt();

	PinSet pin;
	const uint8_t MAX_PWM = 255, MIN_PWM = 0;
private:

};

class Wheel : public Car
{
public:
	Wheel(PinSet &pin);
	void move(int d);

private:
	void read_sensor();
	void forward();
	void backward();
	void left(int8_t);
	void right(int8_t);
	static void on_check_point();

	char sensors[10];
	const uint8_t sensors_num[10] = { 2,3,5,6,7,8 };	// PIN 4,5 BREAK !
	const uint8_t max_sen = 6;
	// ----------------

	const uint8_t PITCH = 5;
	const uint8_t TEETH = 60;

	//static bool stop = false;
};

#define SLIDER_DIR_V 0
#define SLIDER_DIR_H 1
class Slider : public Car
{
public:
	Slider(PinSet &pin);
	void move(int d);		
	void setDir(uint8_t dir);	// dir = 'V' (Vertical) or 'H' (Horizontal)
	uint8_t getDir();
private:
	void forward();
	void backward();

	char _dir;

	float speed_v = 4.87;		// cm/s
	float speed_h = 16.65;		// cm/s

	int d_v_total;
	int d_h_total;
};

bool isCheckPoint(PinSet &pin);
#endif

