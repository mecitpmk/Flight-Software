struct __attribute__ ((packed)) t_demo   {
  uint16_t twoBytes;
  uint8_t  oneByte;
  uint32_t fourBytes;
};

struct t_demo myStruct = {0xBEBA, 0xD0, 0xEFBEADDE};

void setup()
{
  Serial.begin(115200);
  
}

const char startMarker   = '<';
const char endMarker     = '>';
const char ACK           = '-';
bool isSended  = false;
bool waitReply = false;
bool ACKReceived = false;
unsigned long myTimer;
void loop()
{

    if (!isSended && !waitReply)
    {
        Serial.write((uint8_t * )&startMarker,sizeof(startMarker));
        Serial.write((uint8_t * )&myStruct,sizeof(myStruct));
        Serial.write((uint8_t * )&endMarker,sizeof(startMarker));
        myTimer = millis();
        waitReply = true;
    }

    if (Serial.available() && !ACKReceived)
    {
        char receivedPayload = Serial.read();
        Serial.println("*--******-*-*-*-*-*");
        Serial.println(receivedPayload);
        if (receivedPayload == ACK)
        {
            isSended = true;
            Serial.println("OKAY I GOT IT.");
            ACKReceived = true;
        } 
    }

    if (millis() - myTimer <= 300)
    {
        waitReply = false;
    }

    
  
  
}