// car.h

#ifndef _CAR_h
#define _CAR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Car
{
public:
	Car(char _sen_lmost, char _max_sen);
	void move( int d = 0 );
	void forward();
	void backward();
	void halt();
	char isCheckPoint();
private:
	void read_sensor();
	void left(char);
	void right(char);

	char sensors[10];
	char sen_lmost, max_sen;
	const char sen_num[10] = { 2,3,5,6,7,8 };
	//unsigned long  t = 0;

};

extern Car car;



#endif

