#include "mectest.h"
#include <string.h>
/*/
    Written by : Mecit Pamuk
            Last Release Date : 2/11/2021 3:14 AM
                Beta V2.1
                    Entegrated With Python Serial
/*/



Communucation::Communucation()
{
    // Normally they will be deleted in there;
    // elapsed_time = millis();
    *old_datas.altPtr = data.altitude;
    elapsed_time = 0;
}

void Communucation::readPressure()
{
    // Adafruit will be in here
    data.pressure = data.pressure  +1;
    *old_datas.presPtr = data.pressure ;


}
void Communucation::readAltitude()
{
    if (manupulationFalling &&  !fixAltitude)
    {
        *(old_datas.altPtr-1) = data.altitude;
        data.altitude = data.altitude -2;
        *old_datas.altPtr = data.altitude;
        setNewStatus();
    }
    else if (!manupulationFalling && !fixAltitude)
    {
        if (data.altitude > 798 && data.altitude < 802)
        {
            manupulationFalling = true;
        }
        *(old_datas.altPtr-1) = data.altitude;
        data.altitude = data.altitude+2;
        *old_datas.altPtr = data.altitude;
        setNewStatus();
    }
    
    // data.altitude = data.altitude+1;
    // *old_datas.altPtr = data.altitude;
    // setNewStatus();

}
void Communucation::setNewStatus()
{
    if (package_number == 1 || data.altitude >= 0 &&  data.altitude < 3 )
    {
        // ilk açıldığında ve yükseklik   =   0 <= yükseklik < 3
        data.SatelleiteStatus = &data.FLIGHT_WAITING;
    }

    else if (package_number != 1 && data.altitude > *(old_datas.altPtr-1) && data.altitude  >= 4)
    {
        // Şimdikik yükseklik  > önceki yükseklik ve  yükselik >= 4
        data.SatelleiteStatus = &data.FLIGHT_RISING;
    }

    else if (package_number != 1 &&  403 > data.altitude && 397 < data.altitude && !seperatedBefore)
    {
        // 395<yükseklik<405 ve dahaönceAyrışmadıYSA
        // Ayrışma Mekanizmasını Devreye Sok.
        data.SatelleiteStatus = &data.FLIGHT_SEPERATING;
        seperatedBefore = true;
    }

    else if (package_number != 1 && data.altitude <*(old_datas.altPtr-1) && seperatedBefore && data.altitude >= 190 && data.altitude <= 210 && !fixAltTrueBefore)
    {
        //      190 < x < 210 // 185
        Serial.println("NOW ALTITUDE IS FIXING!!!!!!!!*********");
        fixAltitude = true;
        fixAltTrueBefore = true;
        data.SatelleiteStatus = &data.PAYLOAD_FIXED_ALT; 
        // Burada hemen motora güç ver çünkü diğer yerlere gidene kadar time elapsed olacak.
    }

    else if (package_number != 1 && data.altitude < *(old_datas.altPtr-1) && seperatedBefore)
    {
        // Önceki yükselik > şimdiyükselik ve dahaönceAyrıştıysa.
        //
        data.SatelleiteStatus = &data.PAYLOAD_FALLING;
    }

    else if (package_number != 1 && seperatedBefore && data.altitude >= 0  && data.altitude < 5)
    {
        // AYrıştıysa ve  0<= Yükseklik < 5
        data.SatelleiteStatus = &data.PAYLOAD_RESCUE;
    }

    else if (package_number != 1 && data.altitude < *(old_datas.altPtr-1))
    {
        // Şimdiki Yükseklik < Önceki Yükseklik.
        data.SatelleiteStatus = &data.FLIGHT_FALLING;
    }
}
void Communucation::readTemperature()
{
    data.temperature = data.temperature +1;
    *old_datas.tempPtr = data.temperature;
}
void Communucation::readTurnNumber()
{
    data.turn_number = data.turn_number+1;
    *old_datas.turnPtr = data.turn_number;
}

void Communucation::readPitch()
{
    data.pitch = data.pitch+1;
    *old_datas.pitchPtr = data.pitch;
}

void Communucation::readRoll()
{
    data.roll = data.roll+1;    
    *old_datas.rollPtr = data.roll;
}

void Communucation::readYaw()
{
    data.yaw = data.yaw+1;
    *old_datas.yawPtr = data.yaw;
}


void Communucation::releasePayload()
{
    // Normally In  this function has servo motors settings.
    // It means that released.
    Serial.println("PAYLOADSALINIYOR");
    if (!releaseCommand)
    {
        //bidaha motoru aktive etmene gerek yok.
        releaseCommand = true;
        // Servo Settings..
        return;
    }
}
void Communucation::sendOnebyOneLine()
{
    
    // manualServiceCheck(); // Service Check before Sending.
    //const char *numbersChar[10] = {"1 ","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 ","10 "};    
    Serial.print("**********\n");
    
    //Serial.flush();
    testingSerial();
    //toWriteSerial(&temporary_string);
    
    return;

}

uint8_t Communucation::waitforResponse(char *receivedBuffer)
{

    
    char *first;
    char *second;
    char* third;

    uint8_t testVar1 = 0;
    uint8_t testVar2 = 0;
    
    uint8_t secondd = 0;

   /* Serial.print("this is String");
    Serial.println(*stringPtr);*/
    
    if (receivedBuffer[0] != '\0' && serialReadCompleted)
    {
        //const uint16_t lengthSTR = stringPtr->length() + 1;
        if (bufferIndex >= 2)
        {
            //char str_array[lengthSTR];
            //stringPtr->toCharArray(str_array,lengthSTR);
            first = subStr(receivedBuffer," ",1);
            second = subStr(receivedBuffer," ",2);
            testVar1 = 1;
            // Bufferindex 6 dan büyük degil de 4 den büyük olsun cunku 0 0 1- olacak.
            if (bufferIndex > 6)
            {
                third = subStr(receivedBuffer, " ", 3);
                if (!videoByteTakedBefore)
                {
                    if (totalVideoBytes == 0)
                    {
                        // iki kere serial.write yazmamın sebebi atoi ile zaman kaybetmemeiz en azından if condition
                        // okuması daha hızlıdır atoi işleminden :D
                        totalVideoBytes = atoi(third);
                        videoByteTakedBefore = true;
                        Serial.write("0 0 1.\n"); // 6 Length uzunlugunda bi data gonderelim ki haberi olsun..
                    }
                    else
                    {
                        // Coktan aldik hala gonderio ...
                        Serial.write("0 0 1.\n"); // 6 Length uzunlugunda bi data gonderelim ki haberi olsun..
                    }
                    
                }
                else
                {
                    //videoHex = "";
                    videoEnabled = true;
                    uint16_t ln = strlen(third);
                    //Serial.println(ln);

                    char* otherPt = third;

                    // [ FADBCCCDDFD ]
                    // F
                    // A
                    // D
                    for (uint16_t i = 0; i < ln; i++)
                    {
                        fromString(*otherPt);
                        otherPt++;
                    }
                    //ReachedByte += videoHex.length();
                    //Serial.flush();
                    //Serial.println(videoHex);
                    //Serial.flush();
                    //videoHex = "";
                }
                
                // Serial.println("TookEd The Video");
            }
            if (!serviceRecall)
            {
                secondd = atoi(second); // Make the program faster if we write in upper line it will convert all the time but now
                                            // it first check condition which is more efficient for the program.
                serviceVariable = secondd;
            }
            /*int fx = atoi(first);
            if (fx == 0)
            {
                reSendComp = true;
            }*/
            testVar2 = 1;
        }
    }
    else
    {
        feedbackChar = 0;
        return 0;
        /*first = '0';
        second = '0';*/
    }
    uint8_t f = 0;
    if (testVar1 == 1 && testVar2 == 1)
    {
        f = atoi(first);
        
    }
    else
    {
        f = 0;
    }
    
    feedbackChar = f;
    if ((serviceVariable == 77 || serviceVariable == 88 || serviceVariable == 99) && serviceRecall == false) // serviceRecall Control variable 
                                                                                                        //provide to donott enter if condition in every time.
    {
        // Şey yapabilrsin , 10 Sec havada kalma hariç direkt olarak function call yapabiliriz belki?
            // Hangisi mantıklı karar ver.  
        serviceRecall = true;
        // Serial.println("77 or 88 or 99");
        Serial.println(serviceVariable);  // Commentleyebiliriz.
    }
    // Serial.print("Value of f is ");
    // Serial.println(f);
    return f;
}


void Communucation::manualServiceCheck(bool fromOutside)
{
    
    if (fromOutside)
    {
        if (serviceVariable == 88) // Manuel Ayrışma...
        {
            releasePayload();
        }
        else if (serviceVariable == 77) // Manuel Tahrik (MOTOR) for [TESTING] PERVANE
        {
            // motorElapsedTime = millis();
            manualMotorActive = true;
            manualmotorActivation(true);
        }
        else if (serviceVariable == 99) // NOT TESTING JUST MANUAL TAHRIK ALL THE TIME
        {
            manualmotorActivation(false);
            manualMotorActive = true;
        }

    }
        
        
    
}
void Communucation::manualmotorActivation(bool fortesting)
{
    if (fortesting)
    {
        // Motors run 10 seconds for testing.
        motorElapsedTime = millis();
        bool tenSecond = false;
        while (millis() - motorElapsedTime <= testMotorsInterval)
        {
            //Serial.println("MANUALMOTOR10");
            if (!tenSecond)
            {
                Serial.println("manualMotor10");
                tenSecond = true;
            }
            getDatas();
            
            // Motors Run for 10 seconds.
        }
        Serial.println("10ManualMotorISEND!!!");
    }
    else
    {
        // MOTOR RUN ALWAYS WITH CONSTANT SPEED [ITS NOT FOR TESTING.]
        Serial.println("MANUALMOTALWAYS");
        // return;
    }
    
    
    return;
}



void Communucation::getDatas()
{
    // manualServiceCheck(); // ServiceCheck
    // elapsed_time = millis(); // ino dosyasındaki mainLp'nin üstündeydi normalde onu sil. Bu kalsın.
    if (millis() - oneHZ >= oneHzInterval)
    {
        incomingDataChar = '0';
        oneHZ = millis();
        reSendComp = false;
        beforeReading = millis(); // added newly
        readAltitude();
        readPressure();
        readTemperature();
        readPitch();
        readRoll();
        readYaw();
        readTurnNumber();
        
        //Serial.flush(); // This is Just for testing Purposes.
        
        sendOnebyOneLine();
        
        afterReading = millis(); // added new
        RemainTime = afterReading - beforeReading; // added new
        Serial.println(1000 - RemainTime);
        
        //Serial.flush();

        memset(serialBuffer, '\0', sizeof(serialBuffer));
        bufferIndex = 0;


        reSendPeriod = millis(); // Sensorler okunduktan sonra 500 ms gonderilme izni verilmeli.
        bool isTimedOut = false;
        //while (millis() - reSendPeriod <= 500)
        uint16_t oldBufferIndex = 0;
        while (millis() - afterReading < 1000-RemainTime)
        {   
            if (Serial.available() > 0)
            {
                while (Serial.available() > 0 && bufferIndex < 100)
                {
                    // 0 0 FDFFDFDDFFDFDFDFDFDFDFDFDFDFDDFFD-
                    incomingDataChar = Serial.read();
                    if (incomingDataChar != '-')
                    {
                        serialBuffer[bufferIndex++] = incomingDataChar;
                        oldBufferIndex++;
                    }
                    else
                    {
                        incomingDataChar = '0';
                        serialReadCompleted = true;
                        break;
                    }
                }
            }            

            if (serialReadCompleted)
            {
                //Serial.flush();
                uint8_t value = waitforResponse(serialBuffer);
                if (value != 0 && feedbackChar != 0)
                {
                    catchingMissedData();
                    //Serial.flush();
                }
                
                //Serial.flush();
                memset(serialBuffer, '\0', sizeof(serialBuffer));
                bufferIndex = 0;
                // clear input Buffer..
                /*while (Serial.available() > 0)
                {
                    Serial.read();
                    Serial.print("clearing..\n");
                }*/
            }

            if (serialReadCompleted && videoEnabled && feedbackChar == 0)
            {
                // delete this if condition unnecessery right now(CODE REPETITION)
                if (15 < Serial.availableForWrite())
                {
                    
                    
                    Serial.write("Vdby---");
                    Serial.print(totalVideoBytes);
                    Serial.write("By-----");
                    Serial.print(ReachedByte);
                    Serial.write("\n");
                    Serial.write("0 0 1\n"); // Video tooked..
                    //Serial.flush();
                    
                }
                else
                {
                    //Serial.flush();
                    
                    Serial.write("Vdby---");
                    Serial.print(totalVideoBytes);
                    Serial.write("By-----");
                    Serial.print(ReachedByte);
                    Serial.write("\n");
                    Serial.write("0 0 1\n"); // Video tooked..
                    //delay(5);
                }
                //delay(5);
                //Serial.flush();
                //cleanInputBuffer();
            }
            serialReadCompleted = false;
            videoEnabled = false;
        }
        
        //Serial.flush();
        Serial.write("SERIAL-ENDED\n");
        package_number = package_number+1;    
        if ((serviceVariable == 77 || serviceVariable == 88 || serviceVariable == 99) && serviceRecall)
        {
            didService = true; // Unnecessary bool
            serviceRecall = false;
            manualServiceCheck(true);
        }
        /*while (Serial.available() > 0)
        {
            Serial.read();
        }*/
        
        //Serial.flush();
        memset(serialBuffer, '\0', sizeof(serialBuffer));
        bufferIndex = 0;
        //cleanInputBuffer();
        /*for (uint16_t ct = 0; ct < 256; ct++)
        {
            Serial.read();
        }*/
    }
}

void Communucation::mainLp()
{

    if (!systemActivated)
    {
        if (Serial.available())
        {
            String st = Serial.readStringUntil('-');
            if (st == "1")
            {
                systemActivated = true;
                oneHZ = millis();
            }
        }
    }
    else
    {
        getDatas();
        if (fixAltitude) // false
        {
            unsigned long altFix = millis();
            while (millis() - altFix <= testMotorsInterval)
            {
                // MotorSPEED INCREASE IN setNewStatus Because Time will be elasped until comes here...
                Serial.println("!!!! ! !! IRTIFA SABIT !!!!!!! !!! !!!");
                getDatas();
            }
            Serial.println("-------------- IRTIFA SABITLEME TAMAMLANDI.-------------");
            fixAltitude = false;
            // Set Motor Speed To Normal IN RIGHT HERE!!.
        }
    }
}


char* Communucation::subStr (char* str, char *delim, const uint8_t index) 
{
    char* act;
    char* sub;
    char *ptr;
    char copy[150];
    uint16_t i;

    // Since strtok consumes the first arg, make a copy
    strcpy(copy, str);

    for (i = 1, act = copy; i <= index; i++, act = NULL) {
        //Serial.print(".");
        sub = strtok_r(act, delim, &ptr);
        if (sub == NULL) break;
    }
    return sub;
}

void Communucation::testingSerial()
{

    Serial.print("1 ");
    Serial.print(TEAM_ID);
    Serial.print(",");

    Serial.print("2 ");
    Serial.print(package_number);
    Serial.print(",");

    Serial.print("3 ");
    Serial.print(data.pressure);
    Serial.print(",");

    Serial.print("4 ");
    Serial.print(data.altitude);
    Serial.print(",");

    Serial.print("5 ");
    Serial.print(data.temperature);
    Serial.print(",");

    Serial.print("6 ");
    Serial.print(*data.SatelleiteStatus);
    Serial.print(",");

    Serial.print("7 ");
    Serial.print(data.pitch);
    Serial.print(",");

    Serial.print("8 ");
    Serial.print(data.roll);
    Serial.print(",");

    Serial.print("9 ");
    Serial.print(data.yaw);
    Serial.print(",");

    Serial.print("10 ");
    Serial.print(data.turn_number);
    Serial.print("\n");


}



void Communucation::cleanInputBuffer()
{
    for (uint16_t ct = 0; ct < 256; ct++) 
    {
        Serial.read();
    }
}
void Communucation::catchingMissedData()
{
    testingSerial();
    
}



void Communucation::fromString(const char& str) {
    //charToByte(str, ret);
    ReachedByte += 4;
    
    // F = 15
    // 0 = 0
    // str = 'F'

   // Directly write in to sd card?
   /* switch (str) {
    case '0':Serial.println("0000"); break;
    case '1':Serial.println("0001"); break;
    case '2':Serial.println("0010"); break;
    case '3':Serial.println("0011"); break;
    case '4':Serial.println("0100"); break;
    case '5':Serial.println("0101"); break;
    case '6':Serial.println("0110"); break;
    case '7':Serial.println("0111"); break;
    case '8':Serial.println("1000"); break;
    case '9':Serial.println("1001"); break;
    case 'A':Serial.println("1010"); break;
    case 'B':Serial.println("1011"); break;
    case 'C':Serial.println("1100"); break;
    case 'D':Serial.println("1101"); break;
    case 'E':Serial.println("1110"); break;
    case 'F':Serial.println("1111"); break;
    }*/
}

