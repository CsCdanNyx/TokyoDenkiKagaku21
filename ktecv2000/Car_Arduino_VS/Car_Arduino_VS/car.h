// car.h

#ifndef _CAR_h
#define _CAR_h

#if defined(ARDUINO) && ARDUINO >= 100

	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


//-------------------------------
#define __COLOR_SENSOR__
#define __WHEEL__
//#define __SLIDER__
//-------------------------------

struct PinSet
{
	// WHEEL , SLIDER
	uint8_t M1, M2;
	uint8_t MOT_1, MOT_2;

	// COLOR_SENSOR
	uint8_t S0, S1, S2, S3, COLOR_OUT, LED;

	// WHEEL
	uint8_t WHEEL_OI;
	uint8_t PITCH;
	uint8_t TEETH;

	// WHEEL , COLOR_SENSOR
	uint8_t ON_CHECK;
};

static void gear();
static void time_tick();

#define WHEEL_MOVE_GO 0
#define WHEEL_MOVE_BACK 1
class Car
{
public:
	Car(PinSet pin);
	void halt() const;
	static bool isOnCheckPoint();
	static void clearCheckPoint();
	static void setCheckPoint();

	volatile bool stop = false;
protected:
	//virtual void move(int d) = 0;

	const PinSet pin;
	const int MAX_PWM = 255, MIN_PWM = 0;
	uint8_t last_dir;
	
private:
	static volatile bool _onCheckPoint;
};

class Wheel : public Car
{

public:
	Wheel(PinSet pin);
	void move(int mode, int d = 1);
	void spin(int dir) const;
	void forward(int) ;
	void backward(int) ;
	void read_sensor();


private:
	//void read_sensor();
	//void forward(int) const;
	//void backward(int) const;
	void left(int) ;
	void right(int) ;
	
	volatile uint8_t sensors[10];
	
	bool dir = false;

	const uint8_t sensors_num[6] = { 33,31,30,32,34,36 };	// PIN 7 BREAK !
	const uint8_t max_sen = 6;
	// ----------------

	const uint8_t PITCH = 5;
	const uint8_t TEETH = 60;

	const float turn_l_half = 0;

};

#define SLIDER_DIR_V 0
#define SLIDER_DIR_H 1

#define SLIDER_MOVE_T 0
#define SLIDER_MOVE_D 1

class Slider : public Car
{
public:
	Slider(PinSet pin);
	void move(uint8_t mode, float dt);
	void setDir(uint8_t dir);	// dir = 'V' (Vertical) or 'H' (Horizontal)
	uint8_t getDir() const;

	const float t_h_pushOrPull = 4.6;
	const float t_v_up = 7.3, t_v_down = 6.8;			// cm/s 

	const float t_v_pickPen = 8.88;
	const float t_h_pickPen = 2.5;
	const float t_v_dropPen = 15.05;
private:
	void forward(uint8_t pwm) const;
	void backward(uint8_t pwm) const;

	uint8_t _dir;

	const float speed_v = 4.87;			// cm/s
	const float speed_h = 16.65;		// cm/s

	const int d_v_total = 1;
	const int d_h_total = 50;

	// [Task] knock down sign-stand 

};

bool isCheckPoint(PinSet pin);

#endif


