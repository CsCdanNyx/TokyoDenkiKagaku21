// printf.h

#ifndef _PRINTF_h
#define _PRINTF_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

//void p(const __FlashStringHelper *fmt, ...);
void printf_serial(char *fmt, ...);

#endif

