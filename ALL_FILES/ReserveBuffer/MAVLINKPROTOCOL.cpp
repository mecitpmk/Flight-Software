#include "MAVLINKPROTOCOL.h"
//#include <string.h>
/*/
    Written by : Mecit Pamuk
            Last Release Date : 2/11/2021 3:14 AM
                Beta V2.1
                    Entegrated With Python Serial
/*/


/*
                THESE HEADER COMES FROM  GCS ////////////////
            M C                 -> Missed Command (Send Packet again, Activate Command if C 88,99,or etc..)
            N C                 -> Nothing Commmand (ALl Packet okay. Activate Command if C 88,99,or etc..)
            V C V_Byte V_Bin    -> Video Command VByte_Len VByte
            VS C V_SIZE_LEN VSIZE -> VIDEOSIZE Command  VideoSize(Byte)

            E C                   -> Echo Command  (Send me Last Buffer I will check it in the ground station) [[**CANCELLED!**]]
            DV C                 -> DELETEVIDEO TXT RESET VIDEO_SIZE [[**IMPLEMENT_LATER**]]
                THESE HEADER COMES FROM  GCS ////////////////
*/

/*
    TO Send Ground Station Commands: (Ground Station'a gonderilecek Commandlar.)
        I 950           -> I = Interval 950 (ms) 950 sec boyunca iletisimdeyiz!
        V 1             -> V = VideoByte 1 Represent Checksum True Reached! , increase binaries pls  , send new data to me .
        V 3             -> V = VideoByte 3 Represent ALL VIDEO BINARIES REACHED.

        V 0             -> V = Videobyte 0 Represent CheckSum False  , DONT INCREASE INDEX RESEND BINARIES AGAIN
        VS 1            -> VS = VideoSize 1 Represent VideoSizeOkay You can start to send VideoBinaries
        VS 0            -> VS = VideoSize 0 Represent VideoSizeResend Please! You must send VideoSize Again!

*/


DATACLASS::DATACLASS()
{
    pressure = 0;
}
OLDDATACLASS::OLDDATACLASS()
{
    uint8_t QQ = 0;
}
Communucation::Communucation()
{
    // Normally they will be deleted in there;b
    // elapsed_time = millis();
    *old_datas.altPtr = data.altitude;
    
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
        // ilk a????ld??????nda ve y??kseklik   =   0 <= y??kseklik < 3
        strcpy(data.FLIGHT_STATUS,"WAITING");
    }

    else if (package_number != 1 && data.altitude > *(old_datas.altPtr-1) && data.altitude  >= 4)
    {
        // ??imdikik y??kseklik  > ??nceki y??kseklik ve  y??kselik >= 4
        
        strcpy(data.FLIGHT_STATUS,"RISING");
    }

    else if (package_number != 1 &&  403 > data.altitude && 397 < data.altitude && !seperatedBefore)
    {
        // 395<y??kseklik<405 ve daha??nceAyr????mad??YSA
        // Ayr????ma Mekanizmas??n?? Devreye Sok.
        
        strcpy(data.FLIGHT_STATUS,"SEPERATING");
        seperatedBefore = true;
    }

    else if (package_number != 1 && data.altitude <*(old_datas.altPtr-1) && seperatedBefore && data.altitude >= 190 && data.altitude <= 210 && !fixAltTrueBefore)
    {
        //      190 < x < 210 // 185
        Serial.println("NOW ALTITUDE IS FIXING!!!!!!!!*********");
        fixAltitude = true;
        fixAltTrueBefore = true;
        
        strcpy(data.FLIGHT_STATUS,"FIXEDALT");
        // Burada hemen motora g???? ver ????nk?? di??er yerlere gidene kadar time elapsed olacak.
    }

    else if (package_number != 1 && data.altitude < *(old_datas.altPtr-1) && seperatedBefore)
    {
        // ??nceki y??kselik > ??imdiy??kselik ve daha??nceAyr????t??ysa.
        //
        
        strcpy(data.FLIGHT_STATUS,"PAYFALL");
    }

    else if (package_number != 1 && seperatedBefore && data.altitude >= 0  && data.altitude < 5)
    {
        // AYr????t??ysa ve  0<= Y??kseklik < 5
        
        strcpy(data.FLIGHT_STATUS,"RESCUE");
    }

    else if (package_number != 1 && data.altitude < *(old_datas.altPtr-1))
    {
        // ??imdiki Y??kseklik < ??nceki Y??kseklik.
        strcpy(data.FLIGHT_STATUS,"FLIGHTFALL");
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


bool Communucation::waitforResponse(char *Buffers)
{
    char *Tag = subStr(Buffers," ", 1);
    COMMAND = atoi(subStr(Buffers," ",2)); 
    // Serial.println(Tag);
    // Serial.println(COMMAND);
    if (!strcmp(Tag,"M"))
    {
        Serial.println("Missed Package Available Send Again!");
        // memset(Buffer, '\0', BUF_SIZE); // Sonradan Eklendi
        sendTelemetries();
        
    }
    else if (!strcmp(Tag,"N"))
    {
        // IF N IS COMED IT MEANS THAT NO MISSED , NO VIDEO BINARY.
            // Can Break the while loop

        // Serial.println("Nothing missed!");
        return true; // Return true for breaking loop
        
    }
    else if (!strcmp(Tag,"VS"))
    {
        // Serial.println("wideo Size Came!");
        int LENGTH = atoi(subStr(Buffers," ", 3));
        char *VIDEO_B = subStr(Buffers," ",4); // Video BinarySize
        VIDEO_SIZE = strtoul(VIDEO_B,NULL,10); // Video BinarySize to Unsigned Long
        if (LENGTH == strlen(VIDEO_B)) // if 3.index Length == VideoBinary Length its okay correct Length!
        {
            // memset(Buffer, '\0', BUF_SIZE); // Sonradan Eklendi
            // Serial.println("VS 1"); //EX: VS 0 4 1234
            strcpy(SendingStringBuffer,"VS 1");
        }
        else
        {
            // memset(Buffer, '\0', BUF_SIZE); // Sonradan Eklendi
            // Serial.println("VS 0");  // EX: VS 0 1 1111 (HATALI SIZE!)
            strcpy(SendingStringBuffer,"VS 0");
        }
        // Serial.println(VIDEO_SIZE);
        // Serial.println(LENGTH);
    }
    else if (!strcmp(Tag , "V"))
    {
        if (VIDEO_SIZE != 0) // Meaning that(else part) ground station you send me Video binary BUT
                                // YOU DIDNT SEND ME VIDEO SIZE ! PLEASE SEND ME VS FIRST!
        {
            if (!videoTransferCompleted)
            {
                // Serial.println("Wideo CAME!");
                uint8_t CHCKSM = atoi(subStr(Buffers," ",3));
                CURRENT_VIDEO_BIN = subStr(Buffers," ",4);
                uint8_t VIDEO_BIN_LENGHT = strlen(CURRENT_VIDEO_BIN);
                // Serial.println(CURRENT_VIDEO_BIN);
                // Serial.println(CHCKSM);
                // Serial.println(strlen(CURRENT_VIDEO_BIN));
                bool COMPLETED = false;
                if (CHCKSM == VIDEO_BIN_LENGHT)
                {
                    REACHED_SIZE += VIDEO_BIN_LENGHT; // Gelen byte kadar ReachedSIZE Arttirildi.
                    if (REACHED_SIZE == VIDEO_SIZE)
                    {
                        Serial.println("ALL VIDEO SIZE REACHED!");
                        strcpy(SendingStringBuffer,"V 3");
                        // Serial.println("V 3");
                        videoTransferCompleted = true;
                        // Telemetri'ye stat?? okey diyecegiz burada.(VideoStatus)
                        strcpy(data.VIDEO_TRANSMISSION_INFO,"EVET"); //Belki bu gondermeye baslandigindan itibaren ki
                                                                        //Bilgidir Onu konu?? dan????.
                    }
                    else
                    {
                        // memset(Buffer, '\0', BUF_SIZE); // sonradan eklendi.
                        // Serial.println("V 1");
                        strcpy(SendingStringBuffer,"V 1");
                    }
                }
                else 
                {
                    // memset(Buffer, '\0', BUF_SIZE); // Sonradan Eklendi.
                    // Serial.println("V 0");
                    strcpy(SendingStringBuffer,"V 0");
                }
                
                
            }
            else
            {
                Serial.println("WideoCOMPLETEDALLRDY?");
            }
        }
        else
        {
            // memset(Buffer, '\0', BUF_SIZE); // Sonradan Eklendi
            // Serial.println("VS 0");
            strcpy(SendingStringBuffer,"VS 0");
        }
    }
    return false;
}


void Communucation::manualServiceCheck()
{
    
    if (COMMAND == 88) // Manuel Ayr????ma...
    {
        releasePayload();
    }
    else if (COMMAND == 77) // Manuel Tahrik (MOTOR) for [TESTING] PERVANE
    {
        manualmotorActivation(true);
    }
    else if (COMMAND == 99) // NOT TESTING JUST MANUAL TAHRIK ALL THE TIME
    {
        manualmotorActivation(false);
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
                Serial.println("manualMotor10"); // THIS IS FOR NOT PRINTING ALL THE TIME
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
    }
    
}



void Communucation::getDatas()
{
    
    if (millis() - oneHZ >= oneHzInterval)
    {
        
        oneHZ = millis();
        beforeReading = millis();
        COMMAND = 0;
        readAltitude();
        readPressure();
        readTemperature();
        readPitch();
        readRoll();
        readYaw();
        readTurnNumber();
        
        
        
        sendTelemetries();
        
     
        memset(Buffer, '\0', sizeof(Buffer));
        memset(SendingStringBuffer, '\0', sizeof(SendingStringBuffer));

        afterReading = millis(); 
        RemainTime = afterReading - beforeReading;
        
        // uint16_t INTERV = 1000-RemainTime;
        INTERV = 1000-RemainTime;

        clearSerialBuffer(); // ADDED NEWLY

        Serial.print("I ");Serial.println(INTERV);
        
        //char incomingDataChar;
        
        while (millis() - afterReading < INTERV)
        {   
            // delay(1);
            if (Serial.available() > 0)
            {
                // delay(10); // 10
                while (Serial.available() > 0 && !Readed)
                {
                    delayMicroseconds(500); // 50 du gal??ba
                    incomingDataChar = Serial.read();
                    if (incomingDataChar == StartMark)
                    {
                        isReading = true;
                        bufferIndex = 0;
                    }
                    else if (isReading)
                    {
                        if (incomingDataChar != EndMark)
                        {
                            Buffer[bufferIndex++] =  incomingDataChar;
                        }
                        else
                        {
                            Buffer[bufferIndex] = '\0';
                            // strcpy(ReserveBuffer,Buffer);
                            // memset(Buffer,'\0',BUF_SIZE);
                            isReading = false;
                            Readed = true;
                        }
                    }
                }
            }            

            if (Readed)
            {
                Readed = false;
                bool CONTINUE = waitforResponse(Buffer);
                // memset(ReserveBuffer, '\0', BUF_SIZE);
                memset(Buffer, '\0', sizeof(Buffer)); // Reservebuffer ?? disable ettigimiz icin bunu boyle yapmak zorunda kald??m.
                clearSerialBuffer(); // Added Newly
                if (SendingStringBuffer[0] != '\0') Serial.println(SendingStringBuffer);
                memset(SendingStringBuffer,'\0',sizeof(SendingStringBuffer));
                Serial.flush();
                // if (CONTINUE)
                // {
                //     break;
                // }
            }
        }
        Serial.println("E"); // communucation ENDED Message.
        clearSerialBuffer(); // Added Newly
        package_number +=1;
        if (COMMAND != 0)
        {
            manualServiceCheck();
        }
        
        
        
    }
}

void Communucation::mainLp()
{

    if (!systemActivated)
    {
        static char STARTS_BUF[2];
        static uint8_t START_BUF_IDX = 0;
        static bool START_READED = false;
        static bool START_PROCESS = false;
        if (Serial.available() > 0)
        {
            while (Serial.available() > 0 && !START_READED)
            {
                char R = Serial.read();
                if (R == StartMark)
                {
                    START_PROCESS = true;
                }
                else if (START_PROCESS)
                {
                    if (R != EndMark)
                    {
                        STARTS_BUF[START_BUF_IDX++] = R;
                    }
                    else
                    {
                        STARTS_BUF[START_BUF_IDX] = '\0';
                        START_READED = true;
                        START_PROCESS = false;
                    }
                }    
            }
        }
        if (START_READED)
        {
            if (!strcmp(STARTS_BUF,"S"))
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
    static char copy[BUF_SIZE];
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

void Communucation::sendTelemetries()
{

    Serial.print("<");
    Serial.print(TEAM_ID);Serial.print(",");Serial.print(package_number);Serial.print(",");
    Serial.print(data.pressure);Serial.print(",");Serial.print(data.altitude);Serial.print(",");
    Serial.print(data.temperature);Serial.print(",");Serial.print(data.FLIGHT_STATUS);Serial.print(",");
    Serial.print(data.pitch);Serial.print(",");Serial.print(data.roll);Serial.print(",");
    Serial.print(data.yaw);Serial.print(",");Serial.print(data.turn_number);Serial.print(",");
    Serial.print(data.VIDEO_TRANSMISSION_INFO);
    Serial.println(">");


}


void Communucation::clearSerialBuffer()
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
}
