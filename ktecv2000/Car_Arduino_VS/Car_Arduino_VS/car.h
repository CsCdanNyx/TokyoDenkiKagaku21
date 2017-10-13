// car.h

#ifndef _CAR_h
#define _CAR_h

#if defined(ARDUINO) && ARDUINO >= 100

	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


//-------------------------------
//#define __COLOR_SENSOR__
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
	void setCheckNum(uint8_t);

	enum {
		CHECK_POINT_0_START,
		CHECK_POINT_1_PICTURE,
		CHECK_POINT_2_KEEP,
		CHECK_POINT_3_dPICTURE,
		CHECK_POINT_4_KNOCK,
		CHECK_POINT_5_KEEP,
		CHECK_POINT_6_lCURVE,
		CHECK_POINT_7_rCURVE,
		CHECK_POINT_8_rCURVE,
		CHECK_POINT_9_BREAK,
		CHECK_POINT_10_BREAK,
		CHECK_POINT_11_BREAK,
		CHECK_POINT_12_rSPIN,
		CHECK_POINT_13_PICK,
		CHECK_POINT_14_wPICTURE,
		CHECK_POINT_15_WRITE,
		CHECK_POINT_16_DROP,
	};

protected:
	//virtual void move(int d) = 0;

	const PinSet pin;
	const int MAX_PWM = 255, MIN_PWM = 0;
	
	uint8_t check_num = 13 - 1;
	uint8_t move_mode;
private:
	static volatile bool _onCheckPoint;
};

#define MOVE_MODE_BIG_TURN			0
#define MOVE_MODE_SMALL_TURN		1
#define MOVE_MODE_GENERAL			2

class Wheel : public Car
{

public:
	Wheel(PinSet pin);
	void move1();
	void brake(unsigned long msec = 30);
	void spin(int dir, uint8_t pwm = 255);
	inline void read_sensor();

private:

	void left() ;
	void right() ;
	void forward(uint8_t pwm = 0);
	void backward(uint8_t pwm = 255);

	bool inCheckPoint();
	bool touch_check_point();

	void setTurnMode(uint8_t);

	//volatile uint8_t sensors[10];
	volatile uint8_t l, r, ch;	 /// numbers in no-line are

	enum 
	{
		FORWARD, BACKWARD, LEFT, RIGHT, LSPIN, RSPIN
	} dir;

	//const uint8_t sensors_num[6] = { 33,31,30,32,34,36 };	// PIN 7 BREAK !
	const uint8_t MAX_SEN = 4;
	// ----------------

	//const uint8_t PITCH = 5;
	//const uint8_t TEETH = 60;

};

#define SLIDER_DIR_V 0
#define SLIDER_DIR_H 1

#define SLIDER_MOVE_T 0
#define SLIDER_MOVE_D 1
#define SLIDER_MOVE_TEST 2
class Slider : public Car
{
public:
	Slider(PinSet pin);
	void move(uint8_t mode, float dt, uint8_t pwm = 255);
	void setDir(uint8_t dir);	// dir = 'V' (Vertical) or 'H' (Horizontal)
	uint8_t getDir() const;

	const float t_h_pushOrPull = 4.6;
	const float t_v_up = 7.3, t_v_down = 6.8;			// cm/s 

	const float t_v_pickPen = 6.17;
	const float t_h_pickPen = 2.1;
	const float t_v_dropPen = 14;
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

#endif


