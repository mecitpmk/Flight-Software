#include "MAVLINKPROTOCOL.h"
#include <string.h>
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
    altitude = 10;   // Yukseklik
    pressure = 0;   //Basınç
    temperature = 0; //Sıcaklık
    turn_number = 0; // Dönüş Sayısı
    pitch = 0; // Pitch
    yaw = 0; // Yaw
    roll = 0; // Roll
    strcpy(FLIGHT_STATUS, "WAITING");
    strcpy(VIDEO_TRANSMISSION_INFO, "HAYIR");
}
OLDDATACLASS::OLDDATACLASS()
{
      
    presPtr = pressure;
    tempPtr = temperature;
    turnPtr = turn_number;
    pitchPtr = pitch;
    yawPtr = yaw;
    rollPtr = roll;
}
Communucation::Communucation()
{
    // Normally they will be deleted in there;b
    // elapsed_time = millis();
    *old_datas.altPtr = data.altitude;
    
}

void Communucation::readPressure(void)
{
    // Adafruit will be in here
    data.pressure = data.pressure  +1;
    *old_datas.presPtr = data.pressure ;


}
void Communucation::readAltitude(void)
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
void Communucation::setNewStatus(void)
{
    if (package_number == 1 || (data.altitude >= 0 &&  data.altitude < 3) )
    {
        // ilk açıldığında ve yükseklik   =   0 <= yükseklik < 3
        strcpy(data.FLIGHT_STATUS,"WAITING");
    }

    else if (package_number != 1 && data.altitude > *(old_datas.altPtr-1) && data.altitude  >= 4)
    {
        // Şimdikik yükseklik  > önceki yükseklik ve  yükselik >= 4
        
        strcpy(data.FLIGHT_STATUS,"RISING");
    }

    else if (package_number != 1 &&  403 > data.altitude && 397 < data.altitude && !seperatedBefore)
    {
        // 395<yükseklik<405 ve dahaönceAyrışmadıYSA
        // Ayrışma Mekanizmasını Devreye Sok.
        
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
        // Burada hemen motora güç ver çünkü diğer yerlere gidene kadar time elapsed olacak.
    }

    else if (package_number != 1 && data.altitude < *(old_datas.altPtr-1) && seperatedBefore)
    {
        // Önceki yükselik > şimdiyükselik ve dahaönceAyrıştıysa.
        //
        
        strcpy(data.FLIGHT_STATUS,"PAYFALL");
    }

    else if (package_number != 1 && seperatedBefore && data.altitude >= 0  && data.altitude < 5)
    {
        // AYrıştıysa ve  0<= Yükseklik < 5
        
        strcpy(data.FLIGHT_STATUS,"RESCUE");
    }

    else if (package_number != 1 && data.altitude < *(old_datas.altPtr-1))
    {
        // Şimdiki Yükseklik < Önceki Yükseklik.
        strcpy(data.FLIGHT_STATUS,"FLIGHTFALL");
    }
}
void Communucation::readTemperature(void)
{
    data.temperature = data.temperature +1;
    *old_datas.tempPtr = data.temperature;
}
void Communucation::readTurnNumber(void)
{
    data.turn_number = data.turn_number+1;
    *old_datas.turnPtr = data.turn_number;
}

void Communucation::readPitch(void)
{
    data.pitch = data.pitch+1;
    *old_datas.pitchPtr = data.pitch;
}

void Communucation::readRoll(void)
{
    data.roll = data.roll+1;    
    *old_datas.rollPtr = data.roll;
}

void Communucation::readYaw(void)
{
    data.yaw = data.yaw+1;
    *old_datas.yawPtr = data.yaw;
}


void Communucation::releasePayload(void)
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


bool Communucation::waitforResponse(void)
{
    
    subStr(Buffer, " ", 1,&Tag);
    subStr(Buffer, " ", 2, &CMMND);
    COMMAND = atoi(CMMND); 
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
        
        subStr(Buffer, " ", 3, &L);
        LENGTH = atoi(L);
        subStr(Buffer, " ", 4 ,  &VIDEO_B);
        
        VIDEO_SIZE = strtoul(VIDEO_B,NULL,10); // Video BinarySize to Unsigned Long
        if (LENGTH == ((int)strlen(VIDEO_B))) // if 3.index Length == VideoBinary Length its okay correct Length! // uint8_t
        {
            // memset(Buffer, '\0', BUF_SIZE); // Sonradan Eklendi
            // Serial.println("VS 1"); //EX: VS 0 4 1234
            strcpy(SendingStringBuffer,"VS 1\n");
        }
        else
        {
            // memset(Buffer, '\0', BUF_SIZE); // Sonradan Eklendi
            // Serial.println("VS 0");  // EX: VS 0 1 1111 (HATALI SIZE!)
            strcpy(SendingStringBuffer,"VS 0\n");
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
                
                subStr(Buffer, " ", 3 , &CHCK_S);
                CHCKSM = atoi(CHCK_S); // uint8_t // BUNU GLOBALDE OLUŞTUR HER SEFERIDNE MEMORY'DE OLUŞTURMASIN.
                subStr(Buffer," ",4, &CURRENT_VIDEO_BIN); 
                VIDEO_BIN_LENGHT = strlen(CURRENT_VIDEO_BIN); // uint8_t  // BUNU GLOBALDE OLUŞTUR HER SEFERIDNE MEMORY'DE OLUŞTURMASIN.
                // Serial.println(CURRENT_VIDEO_BIN);
                // Serial.println(CHCKSM);
                // Serial.println(strlen(CURRENT_VIDEO_BIN));
                if (CHCKSM == VIDEO_BIN_LENGHT)
                {
                    REACHED_SIZE += VIDEO_BIN_LENGHT; // Gelen byte kadar ReachedSIZE Arttirildi.
                    if (REACHED_SIZE == VIDEO_SIZE || REACHED_SIZE >= VIDEO_SIZE)
                    {
                        Serial.println("ALL VIDEO SIZE REACHED!");
                        strcpy(SendingStringBuffer,"V 3\n");
                        // Serial.println("V 3");
                        videoTransferCompleted = true;
                        // Telemetri'ye statü okey diyecegiz burada.(VideoStatus)
                        strcpy(data.VIDEO_TRANSMISSION_INFO,"EVET"); //Belki bu gondermeye baslandigindan itibaren ki
                                                                        //Bilgidir Onu konuş danış.
                    }
                    else
                    {
                        // memset(Buffer, '\0', BUF_SIZE); // sonradan eklendi.
                        // Serial.println("V 1");
                        strcpy(SendingStringBuffer,"V 1\n");
                    }
                }
                else 
                {
                    // memset(Buffer, '\0', BUF_SIZE); // Sonradan Eklendi.
                    // Serial.println("V 0");
                    strcpy(SendingStringBuffer,"V 0\n");
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
            strcpy(SendingStringBuffer,"VS 0\n");
        }
    }
    return false;
}


void Communucation::manualServiceCheck(void)
{
    
    if (COMMAND == 88) // Manuel Ayrışma...
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



void Communucation::getDatas(void)
{
    
    if (millis() - oneHZ >= oneHzInterval)
    {
        //Serial.begin(115200);
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
        //memset(SendingStringBuffer, '\0', sizeof(SendingStringBuffer));
        SendingStringBuffer[0] = '\0';

        afterReading = millis(); 
        RemainTime = afterReading - beforeReading;
        
        // uint16_t INTERV = 1000-RemainTime;
        INTERV = 1000-RemainTime;

        clearSerialBuffer(); // ADDED NEWLY

        Serial.write("I ");Serial.println(INTERV);
        
        //char incomingDataChar;
        
        while (millis() - afterReading < INTERV)
        {   
            // delay(1);
            if (Serial.available() > 0)
            {
                // delay(10); // 10
                while (Serial.available() > 0 && !Readed)
                {
                    //delayMicroseconds(190); // 50 du galıba
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
                            break; // LAST TIME 1MB ULASTIKTAN SONRA BU BREAK I EKLEDIM.
                        }
                    }
                }
            }            

            if (Readed)
            {
                Readed = false;
                /*bool CONTINUE = waitforResponse(Buffer);*/
                waitforResponse();
                // memset(ReserveBuffer, '\0', BUF_SIZE);
                memset(Buffer, '\0', sizeof(Buffer)); // Reservebuffer ı disable ettigimiz icin bunu boyle yapmak zorunda kaldım.
                clearSerialBuffer(); // Added Newly
                if (SendingStringBuffer[0] != '\0') Serial.write(SendingStringBuffer,strlen(SendingStringBuffer));
                //memset(SendingStringBuffer,'\0',sizeof(SendingStringBuffer));
                SendingStringBuffer[0] = '\0';
                Serial.flush();
                // if (CONTINUE)
                // {
                //     break;
                // }
            }
        }
        Serial.write("E\n"); // communucation ENDED Message.
        clearSerialBuffer(); // Added Newly
        package_number +=1;
        if (COMMAND != 0)
        {
            manualServiceCheck();
        }
        //Serial.end();
        
        
    }
}

void Communucation::mainLp(void)
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


void Communucation::subStr (const char* str, const char *delim, const uint8_t index , char **PARAM_CHAR) 
{
    char* act ;
    char* sub = NULL;
    char *ptr;
    static char copy[BUF_SIZE];
    uint8_t i;
    // Since strtok consumes the first arg, make a copy
    strcpy(copy, str);

    for (i = 1, act = copy; i <= index; i++, act = NULL) {
        //Serial.print(".");
        sub = strtok_r(act, delim, &ptr);
        if (sub == NULL) break;
    }
    *PARAM_CHAR = sub;
}

void Communucation::sendTelemetries(void)
{

    Serial.write("<");
    Serial.print(TEAM_ID);Serial.write(",");Serial.print(package_number);Serial.write(",");
    Serial.print(data.pressure);Serial.write(",");Serial.print(data.altitude);Serial.write(",");
    Serial.print(data.temperature);Serial.write(",");Serial.print(data.FLIGHT_STATUS);Serial.write(",");
    Serial.print(data.pitch);Serial.write(",");Serial.print(data.roll);Serial.write(",");
    Serial.print(data.yaw);Serial.write(",");Serial.print(data.turn_number);Serial.write(",");
    Serial.print(data.VIDEO_TRANSMISSION_INFO);
    Serial.write(">\n");


}


void Communucation::clearSerialBuffer(void)
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
}
