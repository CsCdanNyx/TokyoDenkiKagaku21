#include "car.h"
// --------------------------------
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define COLOR_OUT 5
#define CHECK_RED 25	// whether is red (check_point) for color sensor

#define ON_CHECK 9
// --------------------------------
#define M1 12  // R
#define M2 13  // L

#define MOT_1 10
#define MOT_2 11

#define MAX_PWM 255
#define MIN_PWM 0

#define WHEEL_OI 0
// --------------------------------
#define PITCH 5
unsigned long  teeth = 0;
//#define TEETH 60
//#define TIME_PERIOD 0.001
// --------------------------------
#define ARM_VEL_H 0
#define ARM_VEL_V 0

Car::Car(char _sen_lmost, char _max_sen)
{
	pinMode(MOT_1, OUTPUT);
	pinMode(MOT_2, OUTPUT);
	digitalWrite(M1, HIGH);
	digitalWrite(M2, LOW);
	
	for (char i = 0; i < _max_sen; i++)
		pinMode(_sen_lmost + i, INPUT);

	sen_lmost = _sen_lmost;
	max_sen = _max_sen;


	// color detect
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);
	pinMode(COLOR_OUT, INPUT);
	digitalWrite(S0, HIGH);
	digitalWrite(S1, HIGH);

}

void Car::move(int d = 0)
{
	if (d)
	{
		teeth = 0;
		pinMode(WHEEL_OI, INPUT);

		attachInterrupt(digitalPinToInterrupt(WHEEL_OI), gear, RISING);
		//Timer1.initialize(TIME_PERIOD * 1000000);         // initialize timer1, and set a 1/2 second period
		//Timer1.attachInterrupt(time_tick);  // attaches callback() as a timer overflow interrupt

		if (d > 0)
			forward();
		else
		{
			backward();
			d = -d;
		}
		while (1)
		{
			if (teeth*PITCH >= d)
			{
				car.halt();
				detachInterrupt(digitalPinToInterrupt(WHEEL_OI));
				//Timer1.detachInterrupt();
				return;
			}
			delay(200);
		}
	}
	else
	{
		forward();

		while (1)
		{
			read_sensor();
			char l, r;  // numbers in non-line area
			for (l = 0; sensors[l] && l < max_sen / 2; l++);
			for (r = 0; sensors[max_sen - r - 1] && r < max_sen / 2; r++);

			if (isCheckPoint())
			{
				halt();
			}
			else if (l == r)
			{
				//Serial.println("go");
				forward();
			}
			else if (r > l)
			{
				//Serial.println("left");
				left(r - l);
			}
			else
			{
				//Serial.println("right");
				right(l - r);
			}
		}
	}
}

void Car::read_sensor()
{
	for (int i = 0; i < max_sen; i++)
	{
		sensors[i] = digitalRead(sen_num[i]);
		//Serial.print(sensors[i], DEC);
		//Serial.print(" ");
	}
	//Serial.println(" .");
}

void Car::left(char bend)
{
	analogWrite(MOT_1, MAX_PWM);
	switch (bend)
	{
	case 1:
		analogWrite(MOT_2, 150);
		break;
	case 2:
		analogWrite(MOT_2, 70);
		break;
	default:
		analogWrite(MOT_2, MIN_PWM);
		break;
	}
}

void Car::right(char bend)
{
	analogWrite(MOT_2, MAX_PWM);

	switch (bend)
	{
	case 1:
		analogWrite(MOT_2, 150);
		break;
	case 2:
		analogWrite(MOT_2, 70);
		break;
	default:
		analogWrite(MOT_2, MIN_PWM);
		break;
	}

}

void Car::halt(void)
{
	analogWrite(MOT_1, MIN_PWM);
	analogWrite(MOT_2, MIN_PWM);
}

void Car::forward()
{
	digitalWrite(M1, HIGH);
	digitalWrite(M2, LOW);
	analogWrite(MOT_1, MAX_PWM);
	analogWrite(MOT_2, MAX_PWM);
}

void Car::backward()
{
	digitalWrite(M1, LOW);
	digitalWrite(M2, HIGH);
	analogWrite(MOT_1, MAX_PWM);
	analogWrite(MOT_2, MAX_PWM);
}

char Car::isCheckPoint()
{
	int red = 0;
	int green = 0;
	//int blue = 0;

	digitalWrite(S2, LOW);
	digitalWrite(S3, LOW);
	//count OUT, pRed, RED  
	red = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);
	digitalWrite(S3, HIGH);
	//count OUT, pBLUE, BLUE  
	//blue = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);
	digitalWrite(S2, HIGH);
	//count OUT, pGreen, GREEN  
	green = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);
	delay(300);
	return (red < green && red < CHECK_RED);
}

void gear()
{
	teeth++;
}
/*
void time_tick()
{
	t++;
}
*/
Car car(2,6);

