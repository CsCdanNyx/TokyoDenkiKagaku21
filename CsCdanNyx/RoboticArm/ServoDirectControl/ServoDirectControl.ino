#include <Servo.h>
Servo servo1, servo2, servo3, servo4, servo5, servo6, servo7;
void setup()
{
  //**********************initialize servo*****************
//   servo1.attach(2,500,2400);
//   servo2.attach(3,500,2400);
//   servo3.attach(4,500,2400);
//   servo4.attach(5,500,2400);
//   servo5.attach(6,500,2400);
//   servo6.attach(7,500,2400);
 
   
   
//   servo1.write(90);  delay(15);
//   servo2.write(90);  delay(15);
//   servo3.write(90); delay(15);
//   servo4.write(90); delay(15);
//   servo5.write(130);  delay(15);
//   servo6.write(90); delay(15);
   Serial.begin(9600); 
   while(!Serial.available());
   Serial.print("Enter the angle : ");
   Serial.read();
}

int integerValue=0; 
char incomingByte;

void loop()
{
  if (Serial.available() > 0) {   // something came across serial
    integerValue = 0;         // throw away previous integerValue
    while(1) {            // force into a loop until 'n' is received
      incomingByte = Serial.read();
      if (incomingByte == '\n' || incomingByte == '\r' || incomingByte == 10 || incomingByte == 13) break;   // exit the while(1), we're done receiving
      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
      integerValue *= 10;  // shift left 1 decimal place
      // convert ASCII to integer, add, and shift left 1 decimal place
      integerValue = ((incomingByte - 48) + integerValue);
      
    }
   Serial.println(integerValue);
   //servo3.write(integerValue);
  }
  
}
