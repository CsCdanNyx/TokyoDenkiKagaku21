#ifndef __CAR_H__
#define __CAR_H__

#include "Arduino.h"

#define M1 12  // R
#define M2 13  // L

#define MOT_1 10
#define MOT_2 11

#define MAX_PWM 255
#define MIN_PWM 0

class Car
{
public:
    Car(char _sen_lmost, char _max_sen);
    void move();
    void halt();
private:
    void read_sensor();
    void left (char);
    void right (char);
    void forward();

    char sensors[10];
    char sen_lmost,  max_sen;
};
Car::Car(char _sen_lmost, char _max_sen)
{
    pinMode(MOT_1, OUTPUT);
    pinMode(MOT_2, OUTPUT);

    for(char i = 0; i < _max_sen; i++)
        pinMode(_sen_lmost + i, INPUT);

    sen_lmost = _sen_lmost;
    max_sen = _max_sen;
}

void Car::move() 
{
    analogWrite(MOT_1,MAX_PWM);
    analogWrite(MOT_2,MAX_PWM);
    digitalWrite(M1, HIGH);  
    digitalWrite(M2, LOW);

    while(1)
    {
        read_sensor();
        char l, r;  // numbers in non-line area
        for(l = 0, r = 0; l < max_sen/2; )
        {
            if(sensors[l]) l++;
            if(sensors[max_sen - r - 1]) r++;
        }
        if(l == r)
            forward();
        else if(r > l)
            left(r-l);
        else
            right(l-r);
    }
}

void Car::read_sensor()
{
    for(int i = 0; i < max_sen; i++)
            sensors[i] = digitalRead(sen_lmost + i);
}

void Car::left (char bend)
{
    analogWrite(MOT_2,MIN_PWM);
    switch(bend)
    {
        case 1:
            analogWrite(MOT_1,MAX_PWM/3 * 1);
            break;
        case 2:
            analogWrite(MOT_1,MAX_PWM/3 * 2);
            break;
        case 3:
            analogWrite(MOT_1,MAX_PWM);
            break;
    }
}

void Car::right (char bend)
{
    analogWrite(MOT_1,MIN_PWM);

    switch(bend)
    {
        case 1:
            analogWrite(MOT_2,MAX_PWM/3 * 1);
            break;
        case 2:
            analogWrite(MOT_2,MAX_PWM/3 * 2);
            break;
        case 3:
            analogWrite(MOT_2,MAX_PWM);
            break;
    }
     
}

void Car::halt (void)
{
    analogWrite(MOT_1,MIN_PWM);
    analogWrite(MOT_2,MIN_PWM);
}

void Car::forward()
{
    analogWrite(MOT_1,MAX_PWM);
    analogWrite(MOT_2,MAX_PWM);
}

#endif
