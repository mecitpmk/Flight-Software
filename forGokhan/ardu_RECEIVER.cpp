// C++ code
//
#include <string.h>

struct __attribute__ ((packed)) t_demo   {
  uint16_t twoBytes;
  uint8_t  oneByte;
  uint32_t fourBytes;
};

struct t_demo incomingStruct;


void extractPackage(const uint8_t* const holderArray , struct t_demo * const incomingStructPt)
{
    memcpy(incomingStructPt,holderArray,sizeof(*incomingStructPt));
}

void printIncomingStructure(const struct  t_demo * const incomingStructPt)
{
    return;
    // Serial.print("Two Bytes : ");
    // Serial.println(incomingStructPt->twoBytes);
    // Serial.print("One Bytes : ");
    // Serial.println(incomingStructPt->oneByte);
    // Serial.print("Four Bytes : ");
    // Serial.println(incomingStructPt->fourBytes);
}

void sendACKPackage(void)
{
    const uint8_t ACK = '-';
    Serial.write(&ACK,sizeof(ACK));
}

uint8_t incomingArray[sizeof(incomingStruct)];
void setup()
{
  Serial.begin(115200);
  
}



bool isTransferCompleted = false;
const char startMarker   = '<';
const char endMarker     = '>';
const char ACKMarker     = '-';
bool startedToTake       = false;
void loop()
{
//   Serial.println("----------");
  int ct = 0;
  memset(incomingArray , 0 , sizeof(incomingArray));
  while (Serial.available())
  {

  	uint8_t incomingByte = Serial.read();
    if (incomingByte  == startMarker)
    {
        startedToTake = true;
    }
    else if (incomingByte  == endMarker)
    {
        startedToTake = false;
        isTransferCompleted = true;
        break;
    }
    else if (startedToTake)
    {
        incomingArray[ct++] = incomingByte;
        // Serial.print("Incoming byte is : ");
        // Serial.println(incomingByte);
    }

  }
  if (isTransferCompleted)
  {
    extractPackage(incomingArray , &incomingStruct);
    printIncomingStructure(&incomingStruct);
    isTransferCompleted = false;
    Serial.write((uint8_t*)&ACKMarker, sizeof(ACKMarker));

    // delay(200);

  }
  
  
  
 
  
}