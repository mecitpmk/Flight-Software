#include "MAVLINKPROTOCOL.h"


Communucation c;


void setup() {
  Serial.begin(115200);
  c.stringCopies();
  Serial.flush();
  

}

void loop() 
{
  
    c.mainLp();
  

}
