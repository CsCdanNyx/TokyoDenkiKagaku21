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
		uint8_t last[6] = { 0,0,0,0,0,0 };

		uint8_t rt = 0, ct = 0;

		static uint8_t check_num = 13-1;
		static bool inCheck = false;

		bool leave_check_point = false;
		if (mode == WHEEL_MOVE_GO)
		{
			forward(MAX_PWM);
			delay(500);

			while (1)
			{

				//if (rt++ >= read_times)
				{
					//for (int i = 0; i < 6; i++)
					//printf_serial("%d ", sensors[i]);

					//printf_serial(" => ");
					uint8_t l, r;		// numbers in non-line area
											 //for (l = 0; last[l] > read_times/2 && l < max_sen / 2; l++);
											//for (r = 0; last[max_sen - r - 1] > read_times / 2 && r < max_sen / 2; r++);
					static uint8_t ll, lr;
					//noInterrupts();

					//l = digitalRead(sensors_num[0]);
					//l += digitalRead(sensors_num[1]);
					//l += digitalRead(sensors_num[2]);

					//r = digitalRead(sensors_num[3]);
					//r += digitalRead(sensors_num[4]);
					//r += digitalRead(sensors_num[5]);
					//interrupts();

					noInterrupts();
					uint8_t IN_C = PINC;
					interrupts();
					l = (IN_C >> 4) & 1;// PE4
					l += (IN_C >> 6) & 1;// PE5
					l += (IN_C >> 7) & 1;// PE3
					r = (IN_C >> 5) & 1;// PH3
					r += (IN_C >> 3) & 1;// PH5
					r += (IN_C >> 1) & 1;// PH6

					//printf_serial("%d %d\n", l, r);
					if (inCheck)
					{
						if (check_num == 4 )
						{
							check_num++;

							while (isCheckPoint(pin));
							
							inCheck = false;
							backward(MAX_PWM);
							delay(70);
							halt();
							// correct line error
							delay(1000);
							do
							{
								noInterrupts();
								uint8_t IN_C = PINC;
								interrupts();
								l = digitalRead(sensors_num[0]);
								l += digitalRead(sensors_num[1]);
								l += digitalRead(sensors_num[2]);

								r = digitalRead(sensors_num[3]);
								r += digitalRead(sensors_num[4]);
								r += digitalRead(sensors_num[5]);
								
								if (l > r)
								{
									spin('R');
								}
								else if (r > l)
								{
									spin('L');
								}
							}
							while (abs(l-r) > 1);
							if (l > r)
								spin('L');
							else
								spin('R');
							halt();
							delay(100);

							{

								backward(200);
								delay(1000);
								forward(255);
								delay(30);
								halt();
							}
							break;
						}
						// to correct the IR error causing by check point
						else if (l < 3 && r < 3 && abs(l - r) <= 1)
						{
							inCheck = false;
						}
						else
						{
							continue;
						}
					}
					if (l == 3 && r == 3)
					{
						static uint8_t isCheck_accuracy = 0;

						if (isCheck_accuracy++ < 10)
						{
							delay(10);
							continue;
						}


						isCheck_accuracy = 0;
						check_num++;
						//printf_serial("check point %d\n", check_num);
						

						// correct error before entering check point
						delay(100);



						if (check_num == 1)
						{
							{
								forward(MAX_PWM);
								delay(400);
							}
							
							break;
						}
						else if (check_num == 4 || check_num == 14 || check_num == 15)
						{
							int stable = 0;
							forward(200);
							do
							{
								noInterrupts();
								uint8_t IN_C = PINC;
								interrupts();
								l = (IN_C >> 4) & 1;// PE4
								l += (IN_C >> 6) & 1;// PE5
								l += (IN_C >> 7) & 1;// PE3
								r = (IN_C >> 5) & 1;// PH3
								r += (IN_C >> 3) & 1;// PH5
								r += (IN_C >> 1) & 1;// PH6

								if (l + r < 6)
									stable++;
							} while (stable < 3);

							if(check_num == 4)
								inCheck = true;
							continue;
						}
						else if (check_num == 6)
						{
							int bend = (check_num == 12) ? 1 : 0;
							forward(MAX_PWM);
							delay(100);
							left(2);
							do
							{
								noInterrupts();
								uint8_t IN_C = PINC;
								interrupts();
								l = (IN_C >> 4) & 1;// PE4
								l += (IN_C >> 6) & 1;// PE5
								l += (IN_C >> 7) & 1;// PE3

							} while (l > bend);
						}
						else if (check_num == 7)
						{
							forward(MAX_PWM/2);
							delay(500);
							left(2);
							do
							{
								noInterrupts();
								uint8_t IN_C = PINC;
								interrupts();

								r = (IN_C >> 5) & 1;// PH3
								r += (IN_C >> 3) & 1;// PH5
								r += (IN_C >> 1) & 1;// PH6

							} while (r > 2);
						}
						else if (check_num == 8)
						{
							forward(MAX_PWM);
							delay(100);
							right(2);
							do
							{
								noInterrupts();
								uint8_t IN_C = PINC;
								interrupts();
								
								r = (IN_C >> 5) & 1;// PH3
								r += (IN_C >> 3) & 1;// PH5
								r += (IN_C >> 1) & 1;// PH6

							} while (r);
						}
						else if (check_num == 9 || check_num == 10 || check_num == 11)
						{
							forward(MAX_PWM);
							delay(300);
							//int times = 0;
							if(check_num == 9)
								spin('L');
							delay(100);
							//do
							{
								delay(10);
								noInterrupts();
								uint8_t IN_C = PINC;
								interrupts();
								l = (IN_C >> 4) & 1;// PE4
								l += (IN_C >> 6) & 1;// PE5
								l += (IN_C >> 7) & 1;// PE3
								r = (IN_C >> 5) & 1;// PH3
								r += (IN_C >> 3) & 1;// PH5
								r += (IN_C >> 1) & 1;// PH6
								//times++;
								if (l > r)
									spin('R');
								else if (r > l)
									spin('L');
								delay(300);
								continue;

								continue;
									
							};//while (times < 100);

							//times = 0;
							//spin('R');
							//do
							//{
							//	delay(20);
							//	noInterrupts();
							//	uint8_t IN_C = PINC;
							//	interrupts();
							//	l = (IN_C >> 4) & 1;// PE4
							//	l += (IN_C >> 6) & 1;// PE5
							//	l += (IN_C >> 7) & 1;// PE3
							//	r = (IN_C >> 5) & 1;// PH3
							//	r += (IN_C >> 3) & 1;// PH5
							//	r += (IN_C >> 1) & 1;// PH6
							//} while (abs(l - r) > 1 && l+r >= 5);

						}
						else if (check_num == 12)
						{
							forward(200);
							while (!isCheckPoint(pin));
							delay(200);
							spin('R');
							do
							{
								noInterrupts();
								uint8_t IN_C = PINC;
								interrupts();

								r = (IN_C >> 5) & 1;// PH3
								r += (IN_C >> 3) & 1;// PH5
								r += (IN_C >> 1) & 1;// PH6

							} while (r > 1);

						}
						else if (check_num == 2 || check_num == 3 || check_num == 13)
						{
							forward(200);

							int stable = 0;
							do
							{
								noInterrupts();
								uint8_t IN_C = PINC;
								interrupts();
								l = (IN_C >> 4) & 1;// PE4
								l += (IN_C >> 6) & 1;// PE5
								l += (IN_C >> 7) & 1;// PE3
								r = (IN_C >> 5) & 1;// PH3
								r += (IN_C >> 3) & 1;// PH5
								r += (IN_C >> 1) & 1;// PH6

								if (l + r < 6)
									stable++;
								delay(10);
							}
							while (stable < 5);

							//int correct_times = 0;
							//do
							//{
							//	noInterrupts();
							//	uint8_t IN_C = PINC;
							//	interrupts();
							//	l = (IN_C >> 4) & 1;// PE4
							//	l += (IN_C >> 6) & 1;// PE5
							//	l += (IN_C >> 7) & 1;// PE3
							//	r = (IN_C >> 5) & 1;// PH3
							//	r += (IN_C >> 3) & 1;// PH5
							//	r += (IN_C >> 1) & 1;// PH6

							//	if (l == r)
							//		forward(255);
							//	else if (l > r)
							//		right(l - r);
							//	else
							//		left(r - l);
							//	delay(10);
							//} while (correct_times++ < 20);

							if (check_num == 13)
								break;
						}
						// leave task 1
						else if (check_num == 5)
						{

						}
						// put down pen
						else if (check_num == 16)
						{

						}
					}
					else if (l == ll && r == lr);
					else if (l == r)
					{
						//printf_serial("go %d %d\n", l, r);
						forward(MAX_PWM);
					}
					else if (r > l)
					{
						//printf_serial("left %d\n", r-l);
						left(r - l);
					}
					else
					{
						//printf_serial("right %d\n", l-r);
						right(l - r);
					}
					rt = 0;
					ll = l;
					lr = r;
				}
				delayMicroseconds(50);
			}
		}
		else if (mode == WHEEL_MOVE_BACK)
		{
			backward(MAX_PWM);
			delay(1000);
			while (!isCheckPoint(pin));
		}
	
		//delay(200);
		// eliminate inertia
		if (mode == WHEEL_MOVE_GO)
		{
			backward(MAX_PWM);
			delay(30);
			//left(3);
			//delay(30);
		}
		else if (mode == WHEEL_MOVE_BACK)
		{
			forward(MAX_PWM / 4);
			right(3);
			delay(30);
		}
		halt();
		delay(1000);
		return;
	}
}
void Wheel::forward(int pwm) 
{
	if (!dir)
	{
		digitalWrite(pin.M1, HIGH);
		digitalWrite(pin.M2, HIGH);
		dir = true;
	}
	analogWrite(pin.MOT_1, pwm);
	analogWrite(pin.MOT_2, pwm);
}
void Wheel::backward(int pwm) 
{

	digitalWrite(pin.M1, LOW);
	digitalWrite(pin.M2, LOW);
	
	analogWrite(pin.MOT_1, pwm);
	analogWrite(pin.MOT_2, pwm);
	dir = false;
}
void Wheel::read_sensor()
{
#ifdef __WHEEL__
	noInterrupts();
	byte IN_C = PINC;
	interrupts();

	//static const uint8_t threshold = 200;
	//for (int i = 0; i < max_sen; i++)
	{
		//sensors[i] = analogRead(sensors_num[i]);
		//sensors[i] = digitalRead(sensors_num[i]);
		//printf_serial("%u ", sensors[i]);
	}
	//printf_serial("\n");

	sensors[0] = (IN_C >> 4) & 1;// PE4
	sensors[1] = (IN_C >> 6) & 1;// PE5
	sensors[2] = (IN_C >> 7) & 1;// PE3
	sensors[3] = (IN_C >> 5) & 1;// PH3
	sensors[4] = (IN_C >> 3) & 1;// PH5
	sensors[5] = (IN_C >> 1) & 1;// PH6

	for (int i = 0; i < max_sen; i++)
	{
		printf_serial("%d ", sensors[i]);
	}
	printf_serial("\n");
#endif // __WHEEL__
	
}
void Wheel::left(int bend) 
{
	
	//analogWrite(pin.MOT_1, MAX_PWM);
	//analogWrite(pin.MOT_2, 0);

	switch (bend)
	{
	case 1:
		//printf_serial("left 1\n");
		analogWrite(pin.MOT_1, 255);
		analogWrite(pin.MOT_2, 0);
		break;
	case 2:
		//printf_serial("left 2\n");
		analogWrite(pin.MOT_1, MAX_PWM);
		analogWrite(pin.MOT_2, MIN_PWM);

		break;
	default:
		//printf_serial("left >= 3\n");
		digitalWrite(pin.M2, LOW);
		analogWrite(pin.MOT_1, 200);
		analogWrite(pin.MOT_2, 150);
		delay(100);
		analogWrite(pin.MOT_2, MIN_PWM);
		digitalWrite(pin.M2, HIGH);
		break;
	}

}
void Wheel::right(int bend) 
{
	//analogWrite(pin.MOT_2, MAX_PWM);
	//analogWrite(pin.MOT_1, 0);
	//bend = 2;
	switch (bend)
	{
	case 1:
		//printf_serial("right 1\n");
		analogWrite(pin.MOT_2, 255);
		analogWrite(pin.MOT_1, 0);
		break;
	case 2:
		//printf_serial("right 2\n");
		analogWrite(pin.MOT_2, MAX_PWM);
		analogWrite(pin.MOT_1, MIN_PWM);
		break;
	default:
		//printf_serial("right >= 3\n");
		digitalWrite(pin.M1, LOW);
		analogWrite(pin.MOT_2, 200);
		analogWrite(pin.MOT_1, 150);
		delay(100);
		analogWrite(pin.MOT_1, MIN_PWM);
		digitalWrite(pin.M1, HIGH);
		break;
	}
}
void Wheel::spin(int dir) const
{
	if (dir == 'L')
		// left
	{
		digitalWrite(pin.M1, HIGH);
		digitalWrite(pin.M2, LOW);
	}
	else
	{
		digitalWrite(pin.M1, LOW);
		digitalWrite(pin.M2, HIGH);
	}
	analogWrite(pin.MOT_1, 150);
	analogWrite(pin.MOT_2, 150);
	dir = false;
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
	bool back = false;
	if (time < 0)
	{
		time = abs(time);
		back = true;
	}
	Timer1.initialize(0.01 * 1000000);
	Timer1.attachInterrupt(time_tick);

	// move base on time
	if (mode == SLIDER_MOVE_T)
	{
		if (back)
			backward(MAX_PWM);
		else
			forward(MAX_PWM);		
		while (t < (unsigned long)(time * 100));
		if (back)
			forward(MAX_PWM);
		else
			backward(MAX_PWM);
		Timer1.detachInterrupt();
		//printf_serial("Time = %ld\n", t); //////////
		t = 0;
		delay(20);
		halt();

		//printf_serial("Time = %lu (0.001s)\n", t);
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

