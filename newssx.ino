#include <string.h>
//#include <SoftwareSerial.h>
#include "mectest.h"


Communucation c = Communucation();
//SoftwareSerial myX = SoftwareSerial(10,11);
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
//  SoftwareSerial xbee = SoftwareSerial(2,3);
//  c.MyXbee = &xbee;
//  c.MyXbee->begin(115200);
//  myX.begin(9600);


//    c.MyXbee.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  c.mainLp();
//  myX.print("Hello");
}
