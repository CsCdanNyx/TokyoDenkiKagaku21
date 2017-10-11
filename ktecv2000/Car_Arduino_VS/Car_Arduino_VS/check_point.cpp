#include "car.h"
#include "printf.h"

static const int R_RANGE = 90;   
static const int R_G_DIFF = 30;
bool isCheckPoint(PinSet pin)
{
	static int is_stable = 0;

	unsigned int red = 0;
	unsigned int green = 0;
	unsigned int blue = 0;

	pin.S0 = 21;
	pin.S1 = 20;
	pin.S2 = 19;
	pin.S3 = 18;
	pin.COLOR_OUT = 17;

	digitalWrite(pin.S2, LOW);
	digitalWrite(pin.S3, LOW);

	
	//count OUT, pRed, RED  
	red = pulseIn(pin.COLOR_OUT, digitalRead(pin.COLOR_OUT) == HIGH ? LOW : HIGH);

	digitalWrite(pin.S3, HIGH);
	//count OUT, pBLUE, BLUE  
	 
	blue = pulseIn(pin.COLOR_OUT, digitalRead(pin.COLOR_OUT) == HIGH ? LOW : HIGH);
	digitalWrite(pin.S2, HIGH);

	//count OUT, pGreen, GREEN  
	green = pulseIn(pin.COLOR_OUT, digitalRead(pin.COLOR_OUT) == HIGH ? LOW : HIGH);
	//delay(100);
	//printf_serial("%d %d %d\n", red, green, blue);
	//return ((int)(green-red) > R_G_DIFF && red < R_RANGE);
	return (abs(green - red) > R_G_DIFF && blue > 80 && red < 100);

}
