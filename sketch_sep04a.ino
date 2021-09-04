#include "MAVLINKPROTOCOL.h"


Communucation c;


void setup() {
  Serial.begin(115200);
  c.stringCopies();
  

}

void loop() 
{
  
    c.mainLp();
  

}
