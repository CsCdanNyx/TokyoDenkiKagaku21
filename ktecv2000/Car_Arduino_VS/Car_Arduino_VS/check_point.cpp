#include "car.h"
#include "printf.h"

static const int R_RANGE = 90;   
static const int R_G_DIFF = 20;
bool isCheckPoint(const PinSet &pin)
{
	unsigned int red = 0;
	unsigned int green = 0;
	unsigned int blue = 0;

	digitalWrite(pin.S2, LOW);
	digitalWrite(pin.S3, LOW);

	//count OUT, pRed, RED  
	red = pulseIn(pin.COLOR_OUT, digitalRead(pin.COLOR_OUT) == HIGH ? LOW : HIGH);
	//delay(100);
	digitalWrite(pin.S3, HIGH);
	//count OUT, pBLUE, BLUE  
	 
	//blue = pulseIn(pin.COLOR_OUT, digitalRead(pin.COLOR_OUT) == HIGH ? LOW : HIGH);
	//delay(100);
	digitalWrite(pin.S2, HIGH);

	//count OUT, pGreen, GREEN  
	green = pulseIn(pin.COLOR_OUT, digitalRead(pin.COLOR_OUT) == HIGH ? LOW : HIGH);
	//delay(100);
	//printf_serial("%d %d %d\n", red, green, blue);
	return ((int)(green-red) > R_G_DIFF && red < R_RANGE);
}
