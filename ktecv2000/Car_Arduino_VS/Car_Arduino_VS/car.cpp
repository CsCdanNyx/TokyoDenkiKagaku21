#include "car.h"
#include "printf.h"
//#include <Timer1\TimerOne.h>
#include <TimerOne.h>

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
{  //downard maxPWM delay20
	/*digitalWrite(pin.M1, LOW);
	analogWrite(pin.MOT_1, MAX_PWM); delay(20);
	analogWrite(pin.MOT_1, MIN_PWM);
	digitalWrite(pin.M2, LOW);
	analogWrite(pin.MOT_2, MAX_PWM); delay(20);
	analogWrite(pin.MOT_2, MIN_PWM);*/

	digitalWrite(pin.M1, HIGH);
	analogWrite(pin.MOT_1, MAX_PWM); delay(20);
	analogWrite(pin.MOT_1, MIN_PWM);
	digitalWrite(pin.M2, HIGH);
	analogWrite(pin.MOT_2, MAX_PWM); delay(20);
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
	
	//for (uint8_t i = 0; i < max_sen; i++)
		//pinMode(sensors_num[i], INPUT_PULLUP);

	/// color sensor interrupt pin
	//pinMode(pin.ON_CHECK, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(pin.ON_CHECK), setCheckPoint, LOW);
}
void Wheel::move(int mode, int d = 1)
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
		//if (mode == WHEEL_MOVE_GO)
		{
			forward(MAX_PWM);
		}
		//else if (mode == WHEEL_MOVE_BACK)
		{
			//backward(MAX_PWM);
		}
		//delay(100);
		//Car::clearCheckPoint();

	
		const uint8_t read_times = 4;
		const uint8_t color_times = 30;
		uint8_t last[6] = {0,0,0,0,0,0};
		
		uint8_t rt = 0, ct = 0;
		//while (isCheckPoint(pin));

		bool leave_check_point = false;
		while(1)
		{
			//bool isCheckPt;
			
			//bool isCheckPt = isCheckPoint(pin);
			//if (isCheckPt && leave_check_point)
			//{
			//	printf_serial("In check point\n");
			//	d--;
			//	leave_check_point = false;
			//	continue;
			//}
			//else if (!isCheckPt && !leave_check_point)
			//{
			//	printf_serial("Leave check point\n");
			//	leave_check_point = true;
			//}
			
			//read_sensor();
			//for (int j = 0; j < max_sen; j++)
				//last[j] += sensors[j];
			
			//if (rt++ >= read_times)
			{
				//for (int i = 0; i < 6; i++)
					//printf_serial("%d ", sensors[i]);

				//printf_serial(" => ");
				uint8_t l, r;  // numbers in non-line area
				//for (l = 0; last[l] > read_times/2 && l < max_sen / 2; l++);
				//for (r = 0; last[max_sen - r - 1] > read_times / 2 && r < max_sen / 2; r++);

				l = digitalRead(sensors_num[0]);
				l += digitalRead(sensors_num[1]);
				l += digitalRead(sensors_num[2]);

				r = digitalRead(sensors_num[3]);
				r += digitalRead(sensors_num[4]);
				r += digitalRead(sensors_num[5]);

				static uint8_t check = 0;
				static bool inCheck = false;
				if (inCheck)
				{
					if (isCheckPoint(pin))
					{
						break;
					}
					//if (l < 3 && r < 3 && abs(l - r) <= 1)
					//	inCheck = false;
					else
					{
						continue;
					}
				}
				if (l == 3 && r== 3)
				{
					static uint8_t isCheck = 0;

					if (isCheck++ < 5)
					{

						delay(10);
						continue;
					}
					else
					{
						isCheck = 0;
						printf_serial("check point %d\n", check);
					}
					forward(MAX_PWM);
					delay(120);
					if (check == 0 || check == 1)
					{
						//left(3);
					}
					else if (check == 2)
					{
						//right(3);
					}
					check++;
					inCheck = true;
					delay(200);
				}
				else if (l == r)
				{
					//printf_serial("go %d %d\n", l, r);

					//digitalWrite(pin.M1, HIGH);
					//digitalWrite(pin.M2, HIGH);
					//analogWrite(pin.MOT_1, 255);
					//analogWrite(pin.MOT_2, 255);
					forward(MAX_PWM);
				}
				else if (r > l)
				{
					//printf_serial("left %d\n", r-l);
					left(r - l);
					//analogWrite(pin.MOT_1, MAX_PWM);
					//analogWrite(pin.MOT_2, 0);

				}
				else
				{
					//printf_serial("right %d\n", l-r);
					//analogWrite(pin.MOT_1, 0);
					//analogWrite(pin.MOT_2, MAX_PWM);


					right(l - r);

				}
				rt = 0;
				//for (int i = 0; i < max_sen; i++)
					//last[i] = 0;

				/*if (ct >= color_times)
				{
					ct = 0;
					if (isCheckPoint(pin))
						break;
				}*/
			}
			//delay(1);
			delayMicroseconds(100);
			//ct++;
		}
		delay(200);
		// eliminate inertia
		if (mode == WHEEL_MOVE_GO)
		{
			backward(MAX_PWM / 4);
			//delay(10);
			left(3);
			delay(30);
		}
		else if (mode == WHEEL_MOVE_BACK)
		{
		}
		halt();
		return;
	}
}
void Wheel::forward(int pwm) const
{
	static bool dir = false;
	if (!dir)
	{
		digitalWrite(pin.M1, HIGH);
		digitalWrite(pin.M2, HIGH);
		dir = true;
	}
	analogWrite(pin.MOT_1, pwm);
	analogWrite(pin.MOT_2, pwm);
}
void Wheel::backward(int pwm) const
{

	digitalWrite(pin.M1, LOW);
	digitalWrite(pin.M2, LOW);
	
	analogWrite(pin.MOT_1, pwm);
	analogWrite(pin.MOT_2, pwm);

	digitalWrite(pin.M1, HIGH);
	digitalWrite(pin.M2, HIGH);
}
void Wheel::read_sensor()
{
#ifdef __WHEEL__
	//noInterrupts();
	//byte IN_E = PINE, IN_H = PINH;
	//interrupts();

	//static const uint8_t threshold = 200;
	for (int i = 0; i < max_sen; i++)
	{
		//sensors[i] = analogRead(sensors_num[i]);
		sensors[i] = digitalRead(sensors_num[i]);
		//printf_serial("%u ", sensors[i]);
	}
	//printf_serial("\n");

	//sensors[0] = (IN_E >> 4) & 1;// PE4
	//sensors[1] = (IN_E >> 5) & 1;// PE5
	//sensors[2] = (IN_E >> 3) & 1;// PE3
	//sensors[3] = (IN_H >> 3) & 1;// PH3
	//sensors[4] = (IN_H >> 5) & 1;// PH5
	//sensors[5] = (IN_H >> 6) & 1;// PH6

	//for (int i = 0; i < max_sen; i++)
	//{
	//	printf_serial("%d ", sensors[i]);
	//}
	//printf_serial("\n");
#endif // __WHEEL__
	
}
void Wheel::left(int bend) const
{
	
	//analogWrite(pin.MOT_1, MAX_PWM);
	//analogWrite(pin.MOT_2, 0);

	switch (bend)
	{
	case 1:
		//printf_serial("left 1\n");
		analogWrite(pin.MOT_1, 150);
		analogWrite(pin.MOT_2, MIN_PWM);
		break;
	case 2:
		//printf_serial("left 2\n");
		analogWrite(pin.MOT_1, MAX_PWM);
		analogWrite(pin.MOT_2, MIN_PWM);

		break;
	default:
		//printf_serial("left >= 3\n");
		digitalWrite(pin.M2, LOW);
		analogWrite(pin.MOT_1, 250);
		analogWrite(pin.MOT_2, 200);
		delay(30);
		analogWrite(pin.MOT_2, MIN_PWM);
		digitalWrite(pin.M2, HIGH);
		break;
	}

}
void Wheel::right(int bend) const
{
	//analogWrite(pin.MOT_2, MAX_PWM);
	//analogWrite(pin.MOT_1, 0);
	//bend = 2;
	switch (bend)
	{
	case 1:
		//printf_serial("right 1\n");
		analogWrite(pin.MOT_2, 150);
		analogWrite(pin.MOT_1, MIN_PWM);
		break;
	case 2:
		//printf_serial("right 2\n");
		analogWrite(pin.MOT_2, MAX_PWM);
		analogWrite(pin.MOT_1, MIN_PWM);
		break;
	default:
		//printf_serial("right >= 3\n");
		digitalWrite(pin.M1, LOW);
		analogWrite(pin.MOT_2, 250);
		analogWrite(pin.MOT_1, 200);
		delay(30);
		analogWrite(pin.MOT_1, MIN_PWM);
		digitalWrite(pin.M1, HIGH);
		break;
	}
}
void Wheel::spin() const
{
	digitalWrite(pin.M1, HIGH);
	digitalWrite(pin.M2, LOW);
	analogWrite(pin.MOT_1, MAX_PWM);
	analogWrite(pin.MOT_2, MAX_PWM);
}
#endif

#ifdef __SLIDER__
Slider::Slider(PinSet pin) : Car(pin)
{
}
void Slider::move(uint8_t mode, float dt)
// mode : counting time / distance
// dt : time or distance depending on mode
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
	if (mode == SLIDER_MOVE_T)
	{
		backward(MAX_PWM);	
		//8.71  (1-2)  8.43 (2-3)   upward
		//7.13  (3-2)  7.2  (2-1)     downard

		//8.46  (1-2)  8.45 (2-3)   upward
		//7.12  (3-2)  7.11  (2-1)     downard

		//8.46  (1-2)   upward
		//7.11 (2-1) downward

		//5.96   push
		//5.96   pull

		//5.87  push
		//5.92 pull
		while (Serial.read() == -1); // !

		halt();
		delay(1000);
		printf_serial("Time = %lu (0.001s)\n", t);
		t = 0;

		//backward(MAX_PWM);
		//while (t < time * 100);
		//halt();
		//t = 0;
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
	else if (mode == SLIDER_MOVE_D)
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

