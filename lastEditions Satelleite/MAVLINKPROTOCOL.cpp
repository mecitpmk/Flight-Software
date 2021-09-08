#include "MAVLINKPROTOCOL.h"
// #include "SENSORS.h"
// #include "STORAGE.h"
// #include "FS.h" // NOT NECESSARILY NEEDED.. DONT FORGET TO REMOVE ALSO PUT "#pragma once" words into header files.

#define TRUE 0b1 
#define FALSE 0b0

//#include <string.h>
//#include <WiFiSerial.h>
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
    if (controlVar.FLAGS.manupulationFalling &&  !controlVar.FLAGS.fixAltitude)
    {
        *(old_datas.altPtr-1) = data.altitude;
        data.altitude = data.altitude -2;
        *old_datas.altPtr = data.altitude;
        //setNewStatus();
    }
    else if (!controlVar.FLAGS.manupulationFalling && !controlVar.FLAGS.fixAltitude)
    {
        if (data.altitude > 798 && data.altitude < 802)
        {
            controlVar.FLAGS.manupulationFalling = true;
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
        // strcpy(data.FLIGHT_STATUS,"WAITING");
        dataPacket.FLIGHT_STATUS  =    0;
    }

    else if (package_number != 1 && data.altitude > *(old_datas.altPtr-1) && data.altitude  >= 4)
    {
        // Şimdikik yükseklik  > önceki yükseklik ve  yükselik >= 4
        
        // strcpy(data.FLIGHT_STATUS,"RISING");
        dataPacket.FLIGHT_STATUS  =    1;
    }

    else if (package_number != 1 &&  403 > data.altitude && 397 < data.altitude && !controlVar.FLAGS.seperatedBefore)
    {
        // 395<yükseklik<405 ve dahaönceAyrışmadıYSA
        // Ayrışma Mekanizmasını Devreye Sok.
        
        // strcpy(data.FLIGHT_STATUS,"SEPERATING");
        dataPacket.FLIGHT_STATUS  =    2;
        controlVar.FLAGS.seperatedBefore = TRUE;

    }

    else if (package_number != 1 && data.altitude <*(old_datas.altPtr-1) && controlVar.FLAGS.seperatedBefore && data.altitude >= 190 && data.altitude <= 210 && !controlVar.FLAGS.fixAltitudeBefore)
    {
        //      190 < x < 210 // 185
        Serial.println("NOW ALTITUDE IS FIXING!!!!!!!!*********");
        controlVar.FLAGS.fixAltitude = TRUE;
        controlVar.FLAGS.fixAltitudeBefore = TRUE;
        dataPacket.FLIGHT_STATUS  =    5;
        // Burada hemen motora güç ver çünkü diğer yerlere gidene kadar time elapsed olacak.
    }

    else if (package_number != 1 && data.altitude < *(old_datas.altPtr-1) && controlVar.FLAGS.seperatedBefore)
    {
        // Önceki yükselik > şimdiyükselik ve dahaönceAyrıştıysa.
        //
        
        // strcpy(data.FLIGHT_STATUS,"PAYFALL");
        dataPacket.FLIGHT_STATUS  =    4;
    }

    else if (package_number != 1 && controlVar.FLAGS.seperatedBefore && data.altitude >= 0  && data.altitude < 5)
    {
        // AYrıştıysa ve  0<= Yükseklik < 5
        // strcpy(data.FLIGHT_STATUS,"RESCUE");
        dataPacket.FLIGHT_STATUS  =    6;
    }

    else if (package_number != 1 && data.altitude < *(old_datas.altPtr-1))
    {
        // Şimdiki Yükseklik < Önceki Yükseklik.
        // strcpy(data.FLIGHT_STATUS,"FLIGHTFALL");
        dataPacket.FLIGHT_STATUS  =    3;
    }
}
void Communucation::readTemperature(void)
{
//   data.temperature = (float)sensors.bmpData.temperature; // An example read from sensor, beware that this just reads the existing data and a flush before a read is necessary to get new data.
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
    
    unsigned long manualReleaseTimer = millis();
    // ESC.write(50);
    while (millis() - manualReleaseTimer < 50)
    {
        continue;
    }
    // ESC.write(0);
    
}


bool Communucation::waitforResponse(void)
{
    // getProtocolStatus();
    COMMAND = gcsPacket.command;  // get Command.
    switch (HEADER)
    {
        case MISSED_DATA_AV_H:
            ACKPacket.ACKType = 3;
            ACKPacket.ACK = 1;
            sendTelemetries(); // SEND TELEMETRIES AGAIN.
            break;
        case NOTHING_MISSED_H:
            ACKPacket.ACKType = 3;
            ACKPacket.ACK = 4;
            break; // DO NOTHING
        case VIDEO_SIZE_H:
            // Serial.print("V_S Buffer : ");
            // Serial.println(gcsPacket.bufferArray);
            VIDEO_SIZE = strtoul((const char*)gcsPacket.bufferArray, NULL, 10); // Video BinarySize to Unsigned Long
            // strcpy(SendingStringBuffer, "VS 1\n");
            ACKPacket.ACKType = 0;
            ACKPacket.ACK = 1;
            // Serial.print("Video Size Reached.. ");
            //Serial.println(VIDEO_SIZE);
            break;
        case VIDEO_DATA_H:
            REACHED_SIZE += (int)strlen((const char*)gcsPacket.bufferArray);
            //storage.writeVideo(SD,gcsPacket.bufferArray);
            ACKPacket.ACKType = 1;
            if (REACHED_SIZE >= VIDEO_SIZE)
            {
                // strcpy(SendingStringBuffer, "V 3\n");
                ACKPacket.ACK = 2;
            }
            else
            {
                ACKPacket.ACK = 1;
                // strcpy(SendingStringBuffer, "V 1\n");
                // Serial.println("V1");
                //Serial.println((uint16_t)strlen(Buffer));
            }
            break;  
        case ERROR_H : 
            ACKPacket.ACKType = 3;
            ACKPacket.ACK = 0;
            //Serial.println("ERROR HAPPENED!");
            break;
        default:
            //Serial.println("Default Case ?");
            break;

    }
    return false;
}


void Communucation::sendPackage(void)
{
    // send InformationFrame
    // send dataFrame
    // Serial.beginPacket(SerialAddress, SerialPort);
    // while (true)
    // {
    //     if (Serial.availableForWrite() >= (uint8_t)sizeof(dataPacket))
    //     {

    //         Serial.write((const uint8_t * )&dataPacket,sizeof(dataPacket));
    //         Serial.flush();
    //         break;
    //     }
    //     else Serial.flush();
    // }
    // const char endString[2] = {'\n','\r'};

    Serial.write((const uint8_t * )&dataPacket,sizeof(dataPacket));
    // Serial.println(writed);

    // const uint8_t * const u8Pt = (const uint8_t * const )&dataPacket;
    // uint8_t packageSize = (uint8_t)sizeof(dataPacket);
    // uint8_t writtenData = 0;
    // for (uint8_t i = 0 ; i < packageSize ; i++)
    // {
    //     writtenData += Serial.print(u8Pt[i],DEC);
    //     // writtenData += Serial.write(u8Pt[i]);
    // }
    // Serial.println();
    // Serial.print("Written Data is ");
    // Serial.println(writtenData);    
    
    // Serial.print(">>");

    // Serial.write((const uint8_t * )endString , sizeof(endString));
    // Serial.flush();
    // Serial.endPacket();
}

void Communucation::sendACK(void)
{
     // send InformationFrame   
     // send ACKFrame
    
    // Serial.beginPacket(SerialAddress, SerialPort);
    // Serial.endPacket();
    // while (true)
    // {
    //     if (Serial.availableForWrite() >= (uint8_t)sizeof(ACKPacket))
    //     {

    //         Serial.write((const uint8_t * )&ACKPacket,sizeof(ACKPacket));
    //         Serial.flush();
    //         break;
    //     }
    //     else Serial.flush();
    // }
    // const char endString[2] = {'\n','\r'};

    Serial.write((const uint8_t * )&ACKPacket,sizeof(ACKPacket));
    // Serial.println(writed);
    
    // const uint8_t * const u8Pt = (const uint8_t * const )&ACKPacket;
    // uint8_t packageSize = (uint8_t)sizeof(ACKPacket);
    // uint8_t writtenData = 0;
    // for (uint8_t i = 0 ; i < packageSize ; i++)
    // {
    //     writtenData += Serial.print(u8Pt[i],DEC);
    //     // writtenData += Serial.write(u8Pt[i]);
    // }
    // Serial.println();
    // Serial.print("Written Data is ");
    // Serial.println(writtenData);    
    // Serial.print(">>");
    // Serial.write((const uint8_t *)endString , sizeof(endString));
    // Serial.flush();
    // while (sendedBytes != (uint8_t)sizeof(ACKPacket))
    // {
    //     sendedBytes =  Serial.write((const uint8_t * )&ACKPacket,sizeof(ACKPacket));
    // }
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
        // ESC.write(20);
        while (millis() - motorElapsedTime <= testMotorsInterval)
        {
            //Serial.println("MANUALMOTOR10");
            // if (!tenSecond)
            // {
            //     //Serial.println("manualMotor10"); // THIS IS FOR NOT PRINTING ALL THE TIME
            //     tenSecond = true;
            // }
            getDatas();
            
            // Motors Run for 10 seconds.
        }
        // ESC.write(0);
        //Serial.println("10ManualMotorISEND!!!");
    }
    else
    {
        // MOTOR RUN ALWAYS WITH CONSTANT SPEED [ITS NOT FOR TESTING.]
        //Serial.println("MANUALMOTALWAYS");
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
        // sensors.flushBMPData();
        // After this check Condition about isReleasing or something...
        

        // sensors.flushMPUData(); // Another flush of sensors.
        // Read turn number?


        // sensors.flushGPSData();

        dataPacket.altitude = 32.11;
        dataPacket.pressure = 11.333;
        dataPacket.temperature = 15;
        dataPacket.turn_number = 1;
        dataPacket.pitch = 7.11;
        dataPacket.yaw = -2.11;
        dataPacket.roll = 4.11;
        dataPacket.batteryVoltage = 3.33;
        dataPacket.descentSpeed = 0;
        dataPacket.TEAM_ID = 1152;
        // dataPacket.package_number = 4;
        dataPacket.latitude = 2;
        dataPacket.longtitude = 5;
        
        
        afterReading = millis(); 

        RemainTime = afterReading - beforeReading;
        //dataPacket.Interval= 1000-RemainTime;
        dataPacket.Interval = 700;
        // sendTelemetries();
        sendPackage();
        // delay(50);
        // saveTelemetries(); // Save Telemetries Disabled for Now..
        
        memset(Buffer, '\0', sizeof(Buffer));
        // SendingStringBuffer[0] = '\0';


       
        // Serial.beginPacket(SerialAddress, SerialPort);
        // Serial.printf("I %d", INTERV);
        // Serial.endPacket();
        bufferCt = 0;
        // bool protocolReaded = false;
        // uint8_t testVariable = 0;
        while (millis() - afterReading <  dataPacket.Interval )
        {   
            
            if (!controlVar.FLAGS.Readed) // if (Readed == 0b00000000 || Readed == 0b00000001)
            {
                if (Serial.available())
                {
                    Buffer[bufferCt++] = (uint8_t)Serial.read();
                    if (!controlVar.FLAGS.protocolReaded) // Readed == 0b00000000
                    {
                        getProtocolStatus();
                        // protocolReaded = true ; // Readed =  0b00000001
                        controlVar.FLAGS.protocolReaded = TRUE;
                        continue;
                    }
                    if (bufferCt == MAX_GCS_BYTES)
                    {
                        controlVar.FLAGS.Readed = TRUE;
                        memcpy(&gcsPacket,  Buffer , bufferCt-1); // BU BURDA OLMAMALI
                        memset(Buffer, '\0', sizeof(Buffer));
                        bufferCt    = 0;
                        while (Serial.available())
                        {
                            Serial.read(); // Clean RX buffer..
                        }
                        
                    }
                }
            }

            else
            {
                
                controlVar.FLAGS.Readed = FALSE;
                controlVar.FLAGS.protocolReaded = FALSE;
                waitforResponse();
                if (gcsPacket.bufferArray[0] != '\0')
                {
                    memset(gcsPacket.bufferArray , '\0',sizeof(gcsPacket.bufferArray));
                }
                sendACK();
                // if (SendingStringBuffer[0] != '\0') Serial.write((const uint8_t *) SendingStringBuffer,strlen(SendingStringBuffer)); //we can use Serial.printf %s  ???
                // SendingStringBuffer[0] = '\0';
            }
        }
        ACKPacket.ACKType = 2; // Just make ACK Type 2 (END SIGNAL) 
        ACKPacket.ACK     = 3;
        sendACK();
        
        // Serial.write("E\n"); // communucation ENDED Message.
        package_number +=1;
        dataPacket.package_number += 1;
        if (COMMAND != 0)
        {
            manualServiceCheck();
        }
    }
}

void Communucation::mainLp(void)
{

    if (!controlVar.FLAGS.systemActivated)
    {
        static byte STARTS_BUF[3];
        // static bool START_READED = false;
        // static bool START_READED = true;
        controlVar.FLAGS.startReaded = 1;
        STARTS_BUF[0] = 5;
        
        // Serial.parsePacket();
        int LenBuff = Serial.readBytes(STARTS_BUF, 3);
        // if (STARTS_BUF[0] == 2 )
        // {
        //     const uint8_t myArray[5] = {1,3,5,7,9};
        //     Serial.write(myArray, sizeof(myArray));
        // }
        // Serial.write((const uint8_t * )STARTS_BUF , 1);
        // Serial.print("Length is : : : ");
        // Serial.print(LenBuff);
        
        if (LenBuff > 0)
        {
            // START_READED = true;
            controlVar.FLAGS.startReaded = 1;
            // Serial.println("STarted babba..");
            // for (uint8_t i = 0 ; i < 4 ; i++)
            // {
            //     // Serial.println(STARTS_BUF[i]);
            // }
            // if (STARTS_BUF[0] - '0' == 3)
            // {
            //     // Serial.println("Bu sekildeymis abi..");
            // }
        }
        if (controlVar.FLAGS.startReaded)
        {
            if (STARTS_BUF[0] == 5) // STARTS COMMAND..
            {
                // systemActivated = true;
                controlVar.FLAGS.systemActivated = 1;
                /* ! Newly Added ! Purpose : When the command come from GCS,
                        Direcly make the calibration ESC! 
                        Verified-TESTED : NONE */
                unsigned long manualReleaseTimer = millis();
                // ESC.write(50);
                while (millis() - manualReleaseTimer < 50)
                {
                    continue;
                }
                // ESC.write(0); // ! NEWLY ADDED ! //

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
        if (controlVar.FLAGS.fixAltitude) // false
        {
            unsigned long altFix = millis();
            while (millis() - altFix <= testMotorsInterval)
            {
                // MotorSPEED INCREASE IN setNewStatus Because Time will be elasped until comes here...
                //Serial.println("!!!! ! !! IRTIFA SABIT !!!!!!! !!! !!!");
                getDatas();
            }
            //Serial.println("-------------- IRTIFA SABITLEME TAMAMLANDI.-------------");
            controlVar.FLAGS.fixAltitude = FALSE;
            // fixAltitude = false;
            // Set Motor Speed To Normal IN RIGHT HERE!!.
        }
    }
}



// void Communucation::subStr (void) 
// {
//     char* PT = Buffer;
//     char* p = strtok_r(Buffer, DELIM,&PT); // DELIM GLOBALLY DEFINED.
//     strcpy(TAG, p);
//     uint8_t ct = 0;
//     while (p != NULL)
//     {
//         if (ct == 3)
//         {
//             strcpy(Buffer, p);
//             break;
//         }
//         switch (ct)
//         {
//         case 1:
//             strcpy(CMMND, p);
//             Serial.print("C_CPY,");
//             break;
//         case 2:
//             strcpy(VIDEO_LNG, p);
//             Serial.println("V_L_CP");
//             break;
//         default:
//             break;
//         }
//         p = strtok_r(NULL, DELIM,&PT); // 
//         ct++;
//     }
    
// }

void Communucation::getProtocolStatus(void)
{
    // if (Buffer[0]  == NOTHING_MISSED_H)
    // {    
    //     HEADER = NOTHING_MISSED_H;
    //     memcpy(&gcsPacket,  Buffer , 2); // BU BURDA OLMAMALI

    // } 
    // else if (Buffer[0] == MISSED_DATA_AV_H)
    // {
    //     HEADER = MISSED_DATA_AV_H;
    //     memcpy(&gcsPacket,  Buffer , 2); // BU BURDA OLMAMALI
    // }
    // else if (Buffer[0] == VIDEO_SIZE_H) 
    // {
    //     HEADER = VIDEO_SIZE_H;
    //     memcpy(&gcsPacket,  Buffer , sizeof(gcsPacket)); // BU BURDA OLMAMALI
    // }
    // else if (Buffer[0] == VIDEO_DATA_H)
    // {   
    //     HEADER = VIDEO_DATA_H;
    //     memcpy(&gcsPacket,  Buffer , sizeof(gcsPacket)); // BU BURDA OLMAMALI
    // }
    // else
    // {
    //     HEADER = ERROR_H;
    // }
    switch (Buffer[0])
    {
        case NOTHING_MISSED_H:
            HEADER = NOTHING_MISSED_H;
            MAX_GCS_BYTES = 2;
            // memcpy(&gcsPacket,  Buffer , 2); // BU BURDA OLMAMALI
            break;
        case MISSED_DATA_AV_H:
            HEADER = MISSED_DATA_AV_H;
            MAX_GCS_BYTES = 2;
            // memcpy(&gcsPacket,  Buffer , 2); // BU BURDA OLMAMALI
            break;
        case VIDEO_SIZE_H:
            HEADER = VIDEO_SIZE_H;
            MAX_GCS_BYTES = 104;
            // memcpy(&gcsPacket,  Buffer , sizeof(gcsPacket)); // BU BURDA OLMAMALI
            break;
        case VIDEO_DATA_H:
            HEADER = VIDEO_DATA_H;
            MAX_GCS_BYTES = 104;
            // memcpy(&gcsPacket,  Buffer , sizeof(gcsPacket)); // BU BURDA OLMAMALI
            break;    
        default:
            HEADER = ERROR_H;
            break;
    }
}
void Communucation::stringCopies(void)
{
    // strcpy(data.FLIGHT_STATUS, "WAITING");
    // strcpy(data.VIDEO_TRANSMISSION_INFO, "HAYIR");


    dataPacket.FrameType     = 0xBB; // Says its DataFrame  Normally 0 I Changed // 0xBB 
    ACKPacket.FrameType      = 0xCC; // ACK Frame // Normally 1 I changed to // 0xCC
    gcsPacket.bufferArray[0] = '\0';
    dataPacket.FLIGHT_STATUS = 3; // Normally 0 
    dataPacket.VIDEO_TRANSMISSION_INFO = 5; // Normally 0
    dataPacket.package_number = 1;
    
    ACKPacket.ACKType = 3; // First None
    ACKPacket.ACK = 4;      //first none
    controlVar.resetFlag = FALSE;
}
void Communucation::sendTelemetries(void)
{

    
    // Serial.printf("<");
    // Serial.printf("%d",TEAM_ID);Serial.printf(",");Serial.printf("%d",package_number);Serial.printf(",");
    // Serial.printf("%02d/%02d/%02d %02d:%02d:%02d",sensors.gpsData.day,sensors.gpsData.month,sensors.gpsData.year,sensors.gpsData.hour,sensors.gpsData.minute,sensors.gpsData.second);Serial.printf(",");
    // Serial.printf("%.2f",sensors.bmpData.pressure);Serial.printf(",");Serial.printf("%.2f",sensors.bmpData.altitude);Serial.printf(",");
    // Serial.printf("%.2f",abs(sensors.prevAltitude - sensors.bmpData.altitude));Serial.printf(",");
    // Serial.printf("%.2f",sensors.bmpData.temperature);Serial.printf(",");Serial.printf("%.1f",data.batteryVoltage);Serial.printf(",");
    // Serial.printf("%.2f",sensors.gpsData.latitude);Serial.printf(",");Serial.printf("%.1f",sensors.gpsData.longitude);Serial.printf(",");
    // Serial.printf("%.2f",sensors.gpsData.altitude);Serial.printf(",");
    // Serial.printf("%s",data.FLIGHT_STATUS);Serial.printf(",");
    // Serial.printf("%.2f",sensors.mpuData.pitch);Serial.printf(",");Serial.printf("%.2f",sensors.mpuData.roll);Serial.printf(",");
    // Serial.printf("%.2f",sensors.mpuData.yaw);Serial.printf(",");Serial.printf("%.2f", data.turn_number);Serial.printf(",");
    // Serial.printf("%s",data.VIDEO_TRANSMISSION_INFO);
    // Serial.printf(">\n");


    
}

void Communucation::saveTelemetries(void){
  // Some magic badass telemetry constructor. I now re-evalute my career choices.
//   sprintf(telemetryBuffer,"<%d,%d,%02d/%02d/%02d %02d:%02d:%02d,%.2f,%.2f,%.2f,%.2f,%.1f,%.2f,%.1f,%.2f,%s,%.2f,%.2f,%.2f,%.2f,%s>",
//   TEAM_ID,package_number,sensors.gpsData.day,sensors.gpsData.month,sensors.gpsData.year,sensors.gpsData.hour,sensors.gpsData.minute,sensors.gpsData.second,
//   sensors.bmpData.pressure,sensors.bmpData.altitude,abs(sensors.prevAltitude - sensors.bmpData.altitude),sensors.bmpData.temperature,data.batteryVoltage,
//   sensors.gpsData.latitude,sensors.gpsData.longitude,sensors.gpsData.altitude,data.FLIGHT_STATUS,sensors.mpuData.pitch,sensors.mpuData.roll,sensors.mpuData.yaw,
//   data.turn_number,data.VIDEO_TRANSMISSION_INFO
//   );
//   storage.writeTelemetry(SD,telemetryBuffer);
}
