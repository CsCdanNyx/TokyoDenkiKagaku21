#include "car.h"
#include "TimerOne.h"

#define PITCH 5
#define TEETH 24

#define TIME_PERIOD 0.01 * 1000000

unsigned long teeth = 0;
unsigned long t = 0;
unsigned long s = 0;

char cycle = 3;
Car car(2, 6);

char motor = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(2, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(2), gear, RISING);

    pinMode(10, OUTPUT);
    Timer1.initialize(TIME_PERIOD);         // initialize timer1, and set a 1/2 second period
    Timer1.attachInterrupt(time_count);  // attaches callback() as a timer overflow interrupt
}

void loop() 
{
	while (1)
	{
		if (t >= 100)
		{
			Serial.println(s, DEC);
			t = 0;
		}
	}
	/*
    analogWrite(MOT_1,motor);
    digitalWrite(M1, HIGH);
    analogWrite(MOT_1,MAX_PWM);
    while(1)
    {
        Serial.println(teeth, DEC);
        if(teeth > cycle*TEETH)
        {
            motor_speed();
            analogWrite(MOT_1,0);
            cli();
            while(1) analogWrite(MOT_1,0);;
        }
      
    }
    */   
}

void motor_speed()
{
    int t_total = t * TIME_PERIOD;
    Serial.print("Total time (us) : ");
    Serial.println(t_total, DEC);
    Serial.print("Distance (mm) : ");
    int d = PITCH * teeth;
    Serial.println(d, DEC);
}
void gear()
{
    teeth++;
}
void time_count()
{
    t++;
}
