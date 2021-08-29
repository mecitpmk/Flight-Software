#include "MAVLINKPROTOCOL.h"
#include "SENSORS.h"
//#include <string.h>
//#include <WiFiUdp.h>
/*/
    Written by : Mecit Pamuk
            Last Release Date : 2/11/2021 3:14 AM
                Beta V2.1
                    Entegrated With Python Serial
/*/


/*
                THESE HEADER COMES FROM  GCS ////////////////
            M C                 -> Msed Command (Send Packet again, Activate Command if C 88,99,or etc..)
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
        //setNewStatus();
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
        //setNewStatus();
    }
    setNewStatus();
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
        udp.println("NOW ALTITUDE IS FIXING!!!!!!!!*********");
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
  data.temperature = (float)sensors.bmpData.temperature; // An example read from sensor, beware that this just reads the existing data and a flush before a read is necessary to get new data.
  /*
    data.temperature = data.temperature +1;
    *old_datas.tempPtr = data.temperature;
     */
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
    //udp.printf("PAYLOADSALINIYOR");
    // if (!releaseCommand)
    // {
    //     //bidaha motoru aktive etmene gerek yok.
    //     releaseCommand = true;
    //     // Servo Settings..
    //     return;
    // }
    unsigned long manualReleaseTimer = millis();
    ESC.write(50);
    while (millis() - manualReleaseTimer < 50)
    {
        continue;
    }
    ESC.write(0);
    
}


bool Communucation::waitforResponse(void)
{
    //Serial.print("The Buffer is ");
    //Serial.println(Buffer);
    subStr();
    getProtocolStatus();
    COMMAND = atoi(CMMND);  // get Command.
    switch (HEADER)
    {
        case MISSED_DATA_AV_H:
            sendTelemetries(); // SEND TELEMETRIES AGAIN.
            break;
        case NOTHING_MISSED_H:
            break; // DO NOTHING
        case VIDEO_SIZE_H:
            Serial.print("V_S Buffer : ");
            Serial.println(Buffer);
            VIDEO_SIZE = strtoul(Buffer, NULL, 10); // Video BinarySize to Unsigned Long
            strcpy(SendingStringBuffer, "VS 1\n");
            Serial.print("Video Size Reached.. ");
            Serial.println(VIDEO_SIZE);
            break;
        case VIDEO_DATA_H:
            REACHED_SIZE += (int)strlen(Buffer);
            if (REACHED_SIZE >= VIDEO_SIZE)
            {
                strcpy(SendingStringBuffer, "V 3\n");
            }
            else
            {
                strcpy(SendingStringBuffer, "V 1\n");
                Serial.println("V1");
                //Serial.println((uint16_t)strlen(Buffer));
            }
            break;  
        case ERROR_H : 
            Serial.println("ERROR HAPPENED!");
            break;
        default:
            Serial.println("Default Case ?");
            break;

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
        ESC.write(20);
        while (millis() - motorElapsedTime <= testMotorsInterval)
        {
            //udp.println("MANUALMOTOR10");
            // if (!tenSecond)
            // {
            //     //udp.println("manualMotor10"); // THIS IS FOR NOT PRINTING ALL THE TIME
            //     tenSecond = true;
            // }
            getDatas();
            
            // Motors Run for 10 seconds.
        }
        ESC.write(0);
        //udp.println("10ManualMotorISEND!!!");
    }
    else
    {
        // MOTOR RUN ALWAYS WITH CONSTANT SPEED [ITS NOT FOR TESTING.]
        //udp.println("MANUALMOTALWAYS");
    }
    
}



void Communucation::getDatas(void)
{
    
    if (millis() - oneHZ >= oneHzInterval)
    {
        
        oneHZ = millis();
        beforeReading = millis();
        COMMAND = 0;

        //FIRST flush sensord data so we have recent data.
        sensors.flushBMPData();
        // readAltitude();
        // readPressure();
        // readTemperature();

        sensors.flushMPUData(); // Another flush of sensors.
        // readPitch();
        // readRoll();
        // readYaw();
        // readTurnNumber(); // WTH is turn number, do we calculate this??

        sensors.flushGPSData();
        
        
        udp.beginPacket(udpAddress, udpPort);
        sendTelemetries();
        udp.endPacket();
     
        memset(Buffer, '\0', sizeof(Buffer));
        SendingStringBuffer[0] = '\0';

        afterReading = millis(); 
        RemainTime = afterReading - beforeReading;
        
        INTERV = 1000-RemainTime;

       
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("I %d", INTERV);
        udp.endPacket();
        
        while (millis() - afterReading < INTERV)
        {   
            udp.parsePacket();
            int LenPackage = udp.read(Buffer, 500);
            if (LenPackage > 0)
            {
                Readed = true;
                
            }            

            if (Readed)
            {
                Readed = false;
                waitforResponse();
                memset(Buffer, '\0', sizeof(Buffer)); 
                udp.beginPacket(udpAddress, udpPort);
                if (SendingStringBuffer[0] != '\0') udp.write((const uint8_t *) SendingStringBuffer,strlen(SendingStringBuffer)); //we can use udp.printf %s  ???
                udp.endPacket();
                SendingStringBuffer[0] = '\0';
            }
        }
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("E\n"); // communucation ENDED Message.
        udp.endPacket();
        package_number +=1;
        if (COMMAND != 0)
        {
            manualServiceCheck();
        }
        
        
        
    }
}

void Communucation::mainLp(void)
{

    if (!systemActivated)
    {
        static char STARTS_BUF[10];
        static bool START_READED = false;
        udp.parsePacket();
        int LenBuff = udp.read(STARTS_BUF, 10);

        if (LenBuff > 0)
        {
            START_READED = true;
        }
        if (START_READED)
        {
            if (!strcmp(STARTS_BUF,"S"))
            {
                systemActivated = true; 
                oneHZ = millis();

                // testCalibration = millis();
                // ESC.write(50);
                // while (millis() - testCalibration < 50)
                // {
                //     continue;
                // }
                // ESC.write(0);

            }
        }
    }
    else
    {
      // DO I add the sensor loop here? Maybe gps should be with the others? Maybe not?, Its just that gps update might need to be faster than 1Hz.
        // sensors.flushGPSData(); //Gps Update
        getDatas();
        if (fixAltitude) // false
        {
            unsigned long altFix = millis();
            while (millis() - altFix <= testMotorsInterval)
            {
                // MotorSPEED INCREASE IN setNewStatus Because Time will be elasped until comes here...
                //udp.println("!!!! ! !! IRTIFA SABIT !!!!!!! !!! !!!");
                getDatas();
            }
            //udp.println("-------------- IRTIFA SABITLEME TAMAMLANDI.-------------");
            fixAltitude = false;
            // Set Motor Speed To Normal IN RIGHT HERE!!.
        }
    }
}



void Communucation::subStr (void) 
{
    char* PT = Buffer;
    char* p = strtok_r(Buffer, DELIM,&PT); // DELIM GLOBALLY DEFINED.
    strcpy(TAG, p);
    uint8_t ct = 0;
    while (p != NULL)
    {
        if (ct == 3)
        {
            strcpy(Buffer, p);
            break;
        }
        switch (ct)
        {
        case 1:
            strcpy(CMMND, p);
            Serial.print("C_CPY,");
            break;
        case 2:
            strcpy(VIDEO_LNG, p);
            Serial.println("V_L_CP");
            break;
        default:
            break;
        }
        p = strtok_r(NULL, DELIM,&PT); // 
        ct++;
    }
    
}

void Communucation::getProtocolStatus(void)
{
    if (!strcmp(TAG, "N")) HEADER = NOTHING_MISSED_H;
    else if (!strcmp(TAG, "M")) HEADER = MISSED_DATA_AV_H;
    else if (!strcmp(TAG, "VS")) HEADER = VIDEO_SIZE_H;
    else if (!strcmp(TAG, "V")) HEADER = VIDEO_DATA_H;
    else HEADER = ERROR_H;
}
void Communucation::stringCopies(void)
{
    strcpy(data.FLIGHT_STATUS, "WAITING");
    strcpy(data.VIDEO_TRANSMISSION_INFO, "HAYIR");
}
void Communucation::sendTelemetries(void)
{

    udp.printf("<");
    udp.printf("%d",TEAM_ID);udp.printf(",");udp.printf("%d",package_number);udp.printf(",");
    udp.printf("%02d/%02d/%02d %02d:%02d:%02d",sensors.gpsData.day,sensors.gpsData.month,sensors.gpsData.year,sensors.gpsData.hour,sensors.gpsData.minute,sensors.gpsData.second);udp.printf(",");
    udp.printf("%.2f",sensors.bmpData.pressure);udp.printf(",");udp.printf("%.2f",sensors.bmpData.altitude);udp.printf(",");
    udp.printf("%.2f",abs(sensors.prevAltitude - sensors.bmpData.altitude));udp.printf(",");
    udp.printf("%.2f",sensors.bmpData.temperature);udp.printf(",");udp.printf("%.1f",data.batteryVoltage);udp.printf(",");
    udp.printf("%.2f",sensors.gpsData.latitude);udp.printf(",");udp.printf("%.1f",sensors.gpsData.longitude);udp.printf(",");
    udp.printf("%.2f",sensors.gpsData.altitude);udp.printf(",");
    udp.printf("%s",data.FLIGHT_STATUS);udp.printf(",");
    udp.printf("%.2f",sensors.mpuData.pitch);udp.printf(",");udp.printf("%.2f",sensors.mpuData.roll);udp.printf(",");
    udp.printf("%.2f",sensors.mpuData.yaw);udp.printf(",");udp.printf("%.2f", data.turn_number);udp.printf(",");
    udp.printf("%s",data.VIDEO_TRANSMISSION_INFO);
    udp.printf(">\n");


    // udp.printf("<");
    // udp.printf("%d",TEAM_ID);udp.printf(",");udp.printf("%d",package_number);udp.printf(",");
    // udp.printf("%.2f",data.pressure);udp.printf(",");udp.printf("%.2f",data.altitude);udp.printf(",");
    // udp.printf("%.2f",data.temperature);udp.printf(",");udp.printf("%.1f",data.batteryVoltage);udp.printf(",");
    // udp.printf("%s",data.FLIGHT_STATUS);udp.printf(",");
    // udp.printf("%.2f",data.pitch);udp.printf(",");udp.printf("%.2f",data.roll);udp.printf(",");
    // udp.printf("%.2f",data.yaw);udp.printf(",");udp.printf("%.2f", data.turn_number);udp.printf(",");
    // udp.printf("%s",data.VIDEO_TRANSMISSION_INFO);
    // udp.printf(">\n");

}
