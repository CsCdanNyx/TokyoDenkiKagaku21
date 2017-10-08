#include "car.h"
#include "printf.h"
#include <Timer1\TimerOne.h>

static volatile unsigned long teeth = 0;
static volatile unsigned long t = 0;

static void gear()
{
	teeth++;
}
static void time_tick()
{
	t++;
}


volatile bool Car::_onCheckPoint = false;
Car::Car(PinSet pin)
	: pin (pin)
{
	// Initializer list error (object)
}
void Car::halt() const
{
	analogWrite(pin.MOT_1, MIN_PWM);
	analogWrite(pin.MOT_2, MIN_PWM);
}
void Car::setCheckPoint()
{
	static int time = 0;
	if (time > 3)
	{
		_onCheckPoint = true;
		time = 0;
	}
	else
		time++;
}
bool Car::isOnCheckPoint() // inline
{
	return _onCheckPoint;
}
void Car::clearCheckPoint()
{
	_onCheckPoint = false;
}

#ifdef __WHEEL__
Wheel::Wheel(PinSet pin) : Car(pin)
{
	//this->pin.WHEEL_OI = pin.WHEEL_OI;
	
	for (uint8_t i = 0; i < max_sen; i++)
		pinMode(sensors_num[i], INPUT);

	/// color sensor interrupt pin
	//pinMode(pin.ON_CHECK, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(pin.ON_CHECK), setCheckPoint, LOW);
}
void Wheel::move(int mode, int d = 0)
{
	/*
	if (d != DEFAULT_DISTANCE)
	{
		//teeth = _teeth = 0;

		attachInterrupt(digitalPinToInterrupt(pin.WHEEL_OI), gear, RISING);
		//Timer1.initialize(TIME_PERIOD * 1000000);         // initialize timer1, and set a 1/2 second period
		//Timer1.attachInterrupt(time_tick);  // attaches callback() as a timer overflow interrupt

		if (d > 0)
			//left(3);
			forward();
		else
		{
			//left(3);
			backward();
			d = -d;
		}

		d = TEETH*PITCH * 5 * 2;
		while (1)
		{
			
			if (teeth != _teeth)
			{
				oi[oi_peek]++;
				oi_peek = !oi_peek;
				_teeth = teeth;
			}
			if (oi[0] > 30 || oi[1] > 30)
			{
				printf_serial("%d %d\n", oi[0], oi[1]);
				oi[0] = oi[1] = 0;
			}
			
			
			if (teeth*PITCH >= d)
			{
				halt();
				detachInterrupt(digitalPinToInterrupt(pin.WHEEL_OI));
				//Timer1.detachInterrupt();
				return;
			}
			delay(200);
			
		}
	}
	*/
	//else		// line-following mode
	{
		if (mode == WHEEL_MOVE_GO)
		{
			forward(MAX_PWM);
		}
		else if (mode == WHEEL_MOVE_BACK)
		{

		}
		//delay(100);
		Car::clearCheckPoint();

		const uint8_t read_times = 7;
		uint8_t last[6] = {0,0,0,0,0,0};
		uint8_t times = 0;
		//while (!isOnCheckPoint())
		while(!isCheckPoint(pin))
		{
			read_sensor();
			for (int j = 0; j < max_sen; j++)
				last[j] += sensors[j];
			
			if (times++ >= read_times)
			{
				uint8_t l, r;  // numbers in non-line area
				for (l = 0; last[l] > read_times/2 && l < max_sen / 2; l++);
				for (r = 0; last[max_sen - r - 1] > read_times / 2 && r < max_sen / 2; r++);

				if (l == r)
				{
					//printf_serial("Go %d,%d\n", l, r);
					//Serial.println("go");
					forward(MAX_PWM);
				}
				else if (r > l)
				{
					//printf_serial("Left %d,%d\n", l, r);
					left(r - l);
				}
				else
				{
					//printf_serial("Right %d,%d\n", l, r);
					right(l - r);
				}
				times = 0;
				for (int i = 0; i < max_sen; i++)
					last[i] = 0;
			}

			delayMicroseconds(10);
		}
		if (mode == WHEEL_MOVE_GO)
		{
			backward(MAX_PWM / 4);
		}
		else if (mode == WHEEL_MOVE_BACK)
		{
		}
		halt();
		return;
	}
}
void Wheel::forward(uint8_t pwm) const
{
	static bool dir = false;
	//if (!dir)
	{
		digitalWrite(pin.M1, HIGH);
		digitalWrite(pin.M2, LOW);
		dir = true;
	}
	analogWrite(pin.MOT_1, pwm);
	analogWrite(pin.MOT_2, pwm);
}
void Wheel::backward(uint8_t pwm) const
{

	digitalWrite(pin.M1, LOW);
	digitalWrite(pin.M2, HIGH);
	
	analogWrite(pin.MOT_1, pwm);
	analogWrite(pin.MOT_2, pwm);

	digitalWrite(pin.M1, HIGH);
	digitalWrite(pin.M2, LOW);
}
void Wheel::read_sensor()
{
#ifdef __WHEEL__
	noInterrupts();
	byte IN_E = PINE, IN_H = PINH;
	interrupts();

	//for (int i = 0; i < max_sen; i++)
	//{
	//	sensors[i] = digitalRead(sensors_num[i]);
	//	Serial.print(sensors[i], DEC);
	//	Serial.print(" ");
	//}

	//Serial.println("");

	sensors[0] = (IN_E >> 4) & 1;// PE4
	sensors[1] = (IN_E >> 5) & 1;// PE5
	sensors[2] = (IN_E >> 3) & 1;// PE3
	sensors[3] = (IN_H >> 3) & 1;// PH3
	sensors[4] = (IN_H >> 4) & 1;// PH4
	sensors[5] = (IN_H >> 5) & 1;// PH5


	//for (int i = 0; i < max_sen; i++)
	//	printf_serial("%d ", sensors[i]);
	//printf_serial("\n");
#endif // __WHEEL__
	
}
void Wheel::left(int8_t bend) const
{
	
	//analogWrite(pin.MOT_1, MAX_PWM);
	//analogWrite(pin.MOT_2, MIN_PWM);

	switch (bend)
	{
	case 1:
		analogWrite(pin.MOT_1, 150);
		analogWrite(pin.MOT_2, MIN_PWM);
		break;
	case 2:
		analogWrite(pin.MOT_1, MAX_PWM);
		analogWrite(pin.MOT_2, MIN_PWM);
		break;
	default:
		digitalWrite(pin.M2, HIGH);
		analogWrite(pin.MOT_1, 150);
		analogWrite(pin.MOT_2, 100);
		delay(20);
		analogWrite(pin.MOT_2, MIN_PWM);
		digitalWrite(pin.M2, LOW);
		break;
	}

}
void Wheel::right(int8_t bend) const
{
	//analogWrite(pin.MOT_2, MAX_PWM);
	//analogWrite(pin.MOT_1, MIN_PWM);
	switch (bend)
	{
	case 1:
		analogWrite(pin.MOT_2, 150);
		analogWrite(pin.MOT_1, MIN_PWM);
		break;
	case 2:
		analogWrite(pin.MOT_2, MAX_PWM);
		analogWrite(pin.MOT_1, MIN_PWM);
		break;
	default:
		digitalWrite(pin.M1, LOW);
		analogWrite(pin.MOT_2, 150);
		analogWrite(pin.MOT_1, 100);
		delay(20);
		analogWrite(pin.MOT_1, MIN_PWM);
		digitalWrite(pin.M1, HIGH);
		break;
	}
}
void Wheel::spin() const
{
	digitalWrite(pin.M1, HIGH);
	digitalWrite(pin.M2, HIGH);
	analogWrite(pin.MOT_1, MAX_PWM);
	analogWrite(pin.MOT_2, MAX_PWM);
}
#endif

#ifdef __SLIDER__
Slider::Slider(PinSet pin) : Car(pin)
{
}
void Slider::move(uint8_t mode, float dt)
{

	t = 0;
	// set direction
	uint8_t dir = getDir();
	float time, speed;
	if (dir == SLIDER_DIR_V)
	{
		speed = speed_v;
	}
	else if (dir == SLIDER_DIR_V)
	{
		speed = speed_h;
	}
	time = dt;

	Timer1.initialize(0.01 * 1000000);
	Timer1.attachInterrupt(time_tick);

	// move base on time
	if (dt == SLIDER_MOVE_T)
	{
		forward(MAX_PWM);		
		while (t < time * 100);
		halt();
		delay(1000);
		t = 0;

		backward(MAX_PWM);
		while (t < time * 100);
		halt();
		t = 0;

		Timer1.detachInterrupt();
		return;
	}
	/*
	f		b
	4.527   4.616
	4.516	4.672
	4.526	4.674
	*/

	// move base on distance
	else if (dt == SLIDER_MOVE_D)
	{
		if (dt >= 0)
		{
			forward(MAX_PWM);
		}
		else
		{
			backward(MAX_PWM);
			dt = -dt;
		}
			
		while (t * speed < dt * 100);
		Timer1.detachInterrupt();
		halt();
		return;
	}

}
void Slider::forward(uint8_t pwm) const
{
	if (getDir() == SLIDER_DIR_V)
	{
		digitalWrite(pin.M1, HIGH);
		analogWrite(pin.MOT_1, pwm);
	}
	else if (getDir() == SLIDER_DIR_H)
	{
		digitalWrite(pin.M2, HIGH);
		analogWrite(pin.MOT_2, pwm);
	}
}
void Slider::backward(uint8_t pwm) const
{
	if (getDir() == SLIDER_DIR_V)
	{
		digitalWrite(pin.M1, LOW);
		analogWrite(pin.MOT_1, pwm);

	}
	else if (getDir() == SLIDER_DIR_H)
	{
		digitalWrite(pin.M2, LOW);
		analogWrite(pin.MOT_2, pwm);
	}
}
void Slider::setDir(uint8_t dir)
{
	_dir = dir;
}
uint8_t Slider::getDir() const
{
	return _dir;
}
#endif

