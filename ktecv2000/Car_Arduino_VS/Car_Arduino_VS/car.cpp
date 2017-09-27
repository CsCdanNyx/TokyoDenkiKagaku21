#include "car.h"
#include "printf.h"
#include <Timer1\TimerOne.h>

#define DEFAULT_DISTANCE 99999

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


Car::Car(PinSet &pin)
{
	this->pin.M1 = pin.M1;
	this->pin.M2 = pin.M2;
	this->pin.MOT_1 = pin.MOT_1;
	this->pin.MOT_2 = pin.MOT_2;

	pinMode(pin.MOT_1, OUTPUT);
	pinMode(pin.MOT_2, OUTPUT);
	digitalWrite(pin.M1, HIGH);
	digitalWrite(pin.M2, LOW);
}
void Car::halt()
{
	analogWrite(pin.MOT_1, MIN_PWM);
	analogWrite(pin.MOT_2, MIN_PWM);
}

//#ifdef __WHEEL__
Wheel::Wheel(PinSet &pin) : Car(pin)
{
	this->pin.WHEEL_OI = pin.WHEEL_OI;
	
	for (uint8_t i = 0; i < max_sen; i++)
		pinMode(sensors_num[i], INPUT);

	// color sensor interrupt pin
	pinMode(pin.ON_CHECK, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pin.ON_CHECK), on_check_point, CHANGE);
}
void Wheel::move(int d = DEFAULT_DISTANCE)
{
	if (d != DEFAULT_DISTANCE)
	{
		//teeth = _teeth = 0;
		pinMode(pin.WHEEL_OI, INPUT);

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
			/*
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
			*/
			
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
	else		// line-following mode
	{
		forward();

		while (1)
		{
			read_sensor();
			char l, r;  // numbers in non-line area
			for (l = 0; sensors[l] && l < max_sen / 2; l++);
			for (r = 0; sensors[max_sen - r - 1] && r < max_sen / 2; r++);

			/*
			if (isCheckPoint())
			{
				halt();
			}*/
			if (l == r)
			{
				//printf_serial("Go %d,%d\n", l, r);
				//Serial.println("go");
				forward();
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
		}
	}
}
void Wheel::forward()
{
	digitalWrite(pin.M1, HIGH);
	digitalWrite(pin.M2, LOW);
	analogWrite(pin.MOT_1, MAX_PWM);
	analogWrite(pin.MOT_2, MAX_PWM);
}
void Wheel::backward()
{
	digitalWrite(pin.M1, LOW);
	digitalWrite(pin.M2, HIGH);
	analogWrite(pin.MOT_1, MAX_PWM);
	analogWrite(pin.MOT_2, MAX_PWM);
}
void Wheel::read_sensor()
{
	
	for (int i = 0; i < max_sen; i++)
	{
		sensors[i] = digitalRead(sensors_num[i]);
		//Serial.print(sensors[i], DEC);
		//Serial.print(" ");
	}
	//Serial.println(" .");

}
void Wheel::left(int8_t bend)
{
	analogWrite(pin.MOT_1, MAX_PWM);
	analogWrite(pin.MOT_2, MIN_PWM);
/*
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
*/
}
void Wheel::right(int8_t bend)
{
	analogWrite(pin.MOT_2, MAX_PWM);
	analogWrite(pin.MOT_1, MIN_PWM);
	/*
	switch (bend)
	{
	case 1:
		analogWrite(MOT_1, 150);
		break;
	case 2:
		analogWrite(MOT_1, 70);
		break;
	default:
		analogWrite(MOT_1, MIN_PWM);
		break;
	}
	*/
}
/*
void Wheel::on_check_point()
{
	
}
*/
//#endif 

Slider::Slider(PinSet &pin) : Car(pin)
{

}
void Slider::move(int d = DEFAULT_DISTANCE)
{
	t = 0;

	// set direction
	uint8_t dir = getDir();
	Timer1.initialize(0.01 * 1000000);
	Timer1.attachInterrupt(time_tick);
	
	float speed;
	if (dir == SLIDER_DIR_V)
		speed = speed_v;
	else if (dir == SLIDER_DIR_V)
		speed = speed_h;

	// check distance
	if (d == DEFAULT_DISTANCE)
	// **DANGEROOUS**  move whole distance of slider
	{
		if (dir == SLIDER_DIR_V)
			d = d_v_total;
		else if (dir == SLIDER_DIR_V)
			d = d_h_total;
	}
	else if (d > 0)
	{
		forward();
	}
	else
	{
		backward();
		d = -d;
	}

	// to move
	while (1)
	{
		if (t * 100 * speed >= d)
		{
			Timer1.detachInterrupt();
			halt();
			return;
		}
	}
}
void Slider::forward()
{
	if (getDir() == 'V')
	{
		digitalWrite(pin.M1, HIGH);
		digitalWrite(pin.MOT_1, MAX_PWM);
	}
	else if (getDir() == 'H')
	{
		digitalWrite(pin.M2, HIGH);
		digitalWrite(pin.MOT_2, MAX_PWM);
	}
}
void Slider::backward()
{
	if (getDir() == 'V')
	{
		digitalWrite(pin.M1, LOW);
		digitalWrite(pin.MOT_1, MAX_PWM);
	}
	else if (getDir() == 'H')
	{
		digitalWrite(pin.M2, LOW);
		digitalWrite(pin.MOT_2, MAX_PWM);
	}
}
void Slider::setDir(uint8_t dir)
{
	if(dir == SLIDER_DIR_V || dir == SLIDER_DIR_V)
		_dir = dir;
}
uint8_t Slider::getDir()
{
	return _dir;
}

