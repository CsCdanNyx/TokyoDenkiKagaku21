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
	digitalWrite(pin.M1, HIGH);
	digitalWrite(pin.M2, HIGH);

	analogWrite(pin.MOT_1, MIN_PWM);
	analogWrite(pin.MOT_2, MIN_PWM);
}
void Car::setCheckNum(uint8_t ch)
{
	check_num = ch;
	if (check_num <= CHECK_POINT_4_KNOCK || check_num >= CHECK_POINT_12_rSPIN)
		move_mode = MOVE_MODE_GENERAL;
	else if (check_num == CHECK_POINT_7_rCURVE || check_num == CHECK_POINT_8_rCURVE)
		move_mode = MOVE_MODE_BIG_TURN;
	else
		move_mode = MOVE_MODE_SMALL_TURN;

	//check_num++;
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
bool Wheel::inCheckPoint()
{
	uint8_t ch;
	static int stable_check_times = 0;
	while (stable_check_times < 5 && stable_check_times > -5)
	{
		noInterrupts();
		uint8_t IN_C = PINC;
		interrupts();


		ch = (IN_C >> 4) & 1;
		ch += (IN_C >> 1) & 1;

		//printf_serial("%d\n", (ch == 2)); ////////////////
		if (ch == 2)
		{
			stable_check_times++;
		}
		else
		{
			stable_check_times--;
		}
		delay(1);
	}
	stable_check_times = 0;

	if (stable_check_times >= 4 || stable_check_times <= -6)
		return true;
	else
		return false;


}
bool Wheel::touch_check_point()
{
	static uint8_t stables = 0;

	if (l + r == MAX_SEN && stables++ >= 5)
	{
		check_num++;
		stables = 0;
		return true;
	}
	return false;
}
void Wheel::move1()
{
	static uint8_t ll, lr;

	/// leave start check point
	forward();
	delay(200);

	
	while (1)
	{
		read_sensor();

		//printf_serial("%d %d\n", l, r);////////////
		//printf_serial(" => %d\n", ch); ////////////////

		static bool entered_check_point = false;
		/// enter check point
		
		if (entered_check_point)
		{
			//printf_serial("Check num %d\n", check_num); ////////////////
			
			//enter_check_point = false;
			switch (check_num)
			{

			case CHECK_POINT_1_PICTURE:
				forward();
				delay(200);
				brake();
				entered_check_point = false;
				return;
			/// curve to left : turn left until all left-half part is on the line
			case CHECK_POINT_6_lCURVE:
				forward();
				delay(650);
				left();
				do
				{
					read_sensor();
				} while (r);
				entered_check_point = false;
				//forward(f_speed);
				break;
			case CHECK_POINT_7_rCURVE:
				forward();
				delay(1000);
				spin('L', 120);

				do
				{
					read_sensor();
				} while (l + r);
				entered_check_point = false;
				brake();

				move_mode = MOVE_MODE_SMALL_TURN;
				//delay(10);
				break;
			/// curve to right : turn right until all right-half part is on the line
			case CHECK_POINT_8_rCURVE:
				forward();
				delay(450);
				right();
				do
				{
					read_sensor();
				} while (l);
				entered_check_point = false;
				//forward(f_speed);
				break;


			/// keep'er going
			case CHECK_POINT_5_KEEP:
				move_mode = MOVE_MODE_SMALL_TURN;
				break;
			case CHECK_POINT_3_dPICTURE:
				forward();
				delay(200);
				brake();
				entered_check_point = false;

				delay(1500);

				forward();
				delay(100);
				while (!inCheckPoint());

				while (inCheckPoint());

				while (!inCheckPoint());

				// delay(200); // mid
				brake();
				return;
			case CHECK_POINT_2_KEEP:
			case CHECK_POINT_14_wPICTURE:
			case CHECK_POINT_15_WRITE:
				forward();
				if (l + r != MAX_SEN)
				{
					entered_check_point = false;
				}
				break;
			case CHECK_POINT_9_BREAK:
				static bool correct = false;

			
				if (!correct)
				{
					spin('L', 180);
					delay(80);
					brake();
					correct = true;
				}
			case CHECK_POINT_10_BREAK:
			case CHECK_POINT_11_BREAK:
				forward(180);
				delay(100);
				if (l + r != MAX_SEN)
				{
					entered_check_point = false;
				}
				break;

			/// turn right for 90 degrees
			case CHECK_POINT_12_rSPIN:
				forward(200);
				delay(500);
				if (inCheckPoint())
				{
					spin('R', 180);
					do
					{
						read_sensor();
					} while (l);
					brake(10);
					entered_check_point = false;
				}
				move_mode = MOVE_MODE_GENERAL;
				break;
			/// sign-stand
			case CHECK_POINT_4_KNOCK:
				if (inCheckPoint())
				{
					forward();
					//printf_serial("over\n"); ////////////////
					delay(50);
					brake();
					entered_check_point = false;
					return;
				}
				break;
			/// pick up pen
			case CHECK_POINT_13_PICK:
				//printf_serial(".\n"); ////////////////
				if (inCheckPoint())
				{
					forward();
					//printf_serial("over\n"); ////////////////
					delay(50);
					brake();
					entered_check_point = false;
					return;
				}
				break;
			/// put down pen
			case CHECK_POINT_16_DROP:
				forward();
				if (inCheckPoint())
				{
					delay(50);
					brake();
					entered_check_point = false;
					return;
				}

			}
			
			
		}

		/// Touch check point
		if (l + r == MAX_SEN)
		{
			if (!entered_check_point && touch_check_point())
				entered_check_point = true;

			forward();
			delay(10);
			continue;
		}
		
		//enter_check_point = false;

		if (l > r)
		{
			//printf_serial("R");

			right();
			do
			{
				read_sensor();
				if(touch_check_point())
				{
					//printf_serial("Touch Check\n"); //////////
					entered_check_point = true;
					break;
				}
				//delay(10);

			} while (l);
			//forward(f_speed);
		}
		else if (r > l)
		{
			//printf_serial("L");

			left();
			do
			{
				read_sensor();
				if (touch_check_point())
				{
					entered_check_point = true;
					break;
				}
				//delay(10);
			} while (r);
			//forward(f_speed);
		}
		else
		{
			forward();
			delay(10);
		}
		delay(1);
		// delay(1);
	}

	brake();
}
void Wheel::brake(unsigned long msec = 30)
{
	if (dir == FORWARD)
		backward(MAX_PWM);
	else if (dir == BACKWARD)
		forward(MAX_PWM);
	else if (dir == LSPIN)
		spin('R');
	else if (dir == RSPIN)
		spin('L');
	delay(msec);
	halt();
}
void Wheel::setTurnMode(uint8_t mode)
{
	move_mode = mode;
}
void Wheel::forward(uint8_t pwm = 0)
{
	if (pwm)
	{
		if (dir != FORWARD)
		{
			digitalWrite(pin.M1, HIGH);
			digitalWrite(pin.M2, HIGH);
			dir = FORWARD;
		}
		analogWrite(pin.MOT_1, pwm);
		analogWrite(pin.MOT_2, pwm);
	}
	else
	{
		if (dir != FORWARD)
		{
			digitalWrite(pin.M1, HIGH);
			digitalWrite(pin.M2, HIGH);
			dir = FORWARD;
		}
		if (move_mode = MOVE_MODE_GENERAL)
		{
			analogWrite(pin.MOT_1, MAX_PWM);
			analogWrite(pin.MOT_2, MAX_PWM);
		}
		else if (move_mode = MOVE_MODE_SMALL_TURN)
		{
			analogWrite(pin.MOT_1, 200);
			analogWrite(pin.MOT_2, 200);
		}
		else if (move_mode = MOVE_MODE_BIG_TURN)
		{
			analogWrite(pin.MOT_1, 130);
			analogWrite(pin.MOT_2, 130);
		}
	}
}
void Wheel::backward(uint8_t pwm)
{
	if (dir != BACKWARD)
	{
		digitalWrite(pin.M1, LOW);
		digitalWrite(pin.M2, LOW);
		dir = BACKWARD;
	}
	
	analogWrite(pin.MOT_1, pwm);
	analogWrite(pin.MOT_2, pwm);
}
inline void Wheel::read_sensor()
{
	noInterrupts();
	uint8_t IN_C = PINC;
	interrupts();
	//interrupts();
	
	l = (IN_C >> 6) & 1;// PE5
	l += (IN_C >> 7) & 1;// PE3
	r = (IN_C >> 5) & 1;// PH3
	r += (IN_C >> 3) & 1;// PH5

	ch = (IN_C >> 4) & 1;
	ch += (IN_C >> 1) & 1;

	//printf_serial("%d %d %d\n", l, r, ch); /////////
}
void Wheel::left()
{
	uint8_t bend = r - l;
	if(move_mode == MOVE_MODE_GENERAL)
		switch (bend)
		{
		case 1:
			//printf_serial("left 1\n");
			analogWrite(pin.MOT_1, 150);
			analogWrite(pin.MOT_2, 0);
			break;
		default:
			//printf_serial("left 2\n");
			//analogWrite(pin.MOT_1, MAX_PWM);
			analogWrite(pin.MOT_1, 200);
			analogWrite(pin.MOT_2, MIN_PWM);

			break;
		}
	else if(move_mode == MOVE_MODE_SMALL_TURN)
		switch (bend)
		{
		default:
			//printf_serial("left >= 3\n");
			//digitalWrite(pin.M2, LOW);
			analogWrite(pin.MOT_1, 200);
			analogWrite(pin.MOT_2, MIN_PWM);
			//delay(30);
			//analogWrite(pin.MOT_2, MIN_PWM);
			//digitalWrite(pin.M2, HIGH);
			//break;
		}
	else if(move_mode == MOVE_MODE_BIG_TURN)
		switch (bend)
		{
		default:
			analogWrite(pin.MOT_1, 175);
			analogWrite(pin.MOT_2, 0);
		}

	dir = LEFT;
}
void Wheel::right()
{
	uint8_t bend = l - r;
	if(move_mode == MOVE_MODE_GENERAL)
		switch (bend)
		{
		case 1:
			//printf_serial("right 1\n");
			analogWrite(pin.MOT_2, 150);
			analogWrite(pin.MOT_1, MIN_PWM);
			break;
		case 2:
			//printf_serial("right 2\n");
			//analogWrite(pin.MOT_2, MAX_PWM);
			analogWrite(pin.MOT_2, 200);
			analogWrite(pin.MOT_1, MIN_PWM);
			break;
		}
	else if(move_mode == MOVE_MODE_SMALL_TURN)
		switch (bend)
		{
		default:
			analogWrite(pin.MOT_2, 200);
			analogWrite(pin.MOT_1, 0);
			break;
		}
	else if(move_mode == MOVE_MODE_BIG_TURN)
		switch (bend)
		{
		default:
			//printf_serial("right >= 3\n");
			//digitalWrite(pin.M1, LOW);
			analogWrite(pin.MOT_2, 175);
			analogWrite(pin.MOT_1, 0);
			/*delay(100);
			analogWrite(pin.MOT_1, MIN_PWM);
			digitalWrite(pin.M1, HIGH);
			break;*/
		}
	dir = RSPIN;
}
void Wheel::spin(int dir, uint8_t pwm = 255)
{
	if (dir == 'L')
		// left
	{
		digitalWrite(pin.M1, HIGH);
		digitalWrite(pin.M2, LOW);

		dir = LSPIN;
	}
	else
	{
		digitalWrite(pin.M1, LOW);
		digitalWrite(pin.M2, HIGH);

		dir = RSPIN;
	}
	analogWrite(pin.MOT_1, pwm);
	analogWrite(pin.MOT_2, pwm);
	
}
#endif

#ifdef __SLIDER__
Slider::Slider(PinSet pin) : Car(pin)
{
}
void Slider::move(uint8_t mode, float dt, uint8_t pwm = 255)
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
	if (mode == SLIDER_MOVE_T || mode == SLIDER_MOVE_TEST)
	{
		if (back)
			backward(pwm);
		else
			forward(pwm);
		if (mode == SLIDER_MOVE_TEST)
			while (Serial.read() == -1);
		else if(mode == SLIDER_MOVE_T)
			while (t < (unsigned long)(time * 100));

		// PICK PEN V : 5.99, 6.21 [6.17]
		// DROP PEN V : 12.8
		if (back)
			forward(pwm);
		else
			backward(pwm);
		Timer1.detachInterrupt();
		printf_serial("Time = %ld\n", t); //////////
		t = 0;
		delay(25);
		halt();

		//printf_serial("Time = %lu (0.001s)\n", t);
		return;
	}

	// move base on distance
	else if (mode == SLIDER_MOVE_D)
	{
		printf_serial("Position Mode\n");//////
		pinMode(20, INPUT);  // V
		pinMode(21, INPUT);  // H

		if (back)
		{
			backward(pwm);
		}
		else
		{
			forward(pwm);
		}
		
		int pos = 0;
		while (pos < 2)
		{
			static uint8_t last_ri = 1;
			if (digitalRead(20) != last_ri)
			{
				printf_serial("!");
				if (last_ri == 0)
				{
					pos++;
				}
				last_ri = !last_ri;
			}
			delay(20);
		}
		if (back)
			forward(pwm);
		else
			backward(pwm);
		Timer1.detachInterrupt();
		//printf_serial("Time = %ld\n", t); //////////
		t = 0;
		delay(25);
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

