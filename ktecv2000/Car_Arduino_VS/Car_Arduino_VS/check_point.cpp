#include "car.h"

static const int RED_RANGE = 25;

bool isCheckPoint(PinSet &pin)
{
	int red = 0;
	int green = 0;
	//int blue = 0;

	digitalWrite(pin.S2, LOW);
	digitalWrite(pin.S3, LOW);
	//count OUT, pRed, RED  
	red = pulseIn(pin.COLOR_OUT, digitalRead(pin.COLOR_OUT) == HIGH ? LOW : HIGH);
	digitalWrite(pin.S3, HIGH);
	//count OUT, pBLUE, BLUE  
	//blue = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);
	digitalWrite(pin.S2, HIGH);
	//count OUT, pGreen, GREEN  
	green = pulseIn(pin.COLOR_OUT, digitalRead(pin.COLOR_OUT) == HIGH ? LOW : HIGH);
	delay(300);
	return (red < green && red < RED_RANGE);
}