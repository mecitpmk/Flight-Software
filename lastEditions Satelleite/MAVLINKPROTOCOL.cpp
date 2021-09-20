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
    *old_datas.altPtr = dataPacket.altitude;
    
    
}

void Communucation::readPressure(void)
{
    // Adafruit will be in here
    dataPacket.pressure = dataPacket.pressure  +1;
    *old_datas.presPtr = dataPacket.pressure ;


}
void Communucation::readAltitude(void)
{
    if (controlVar.FLAGS.manupulationFalling &&  !controlVar.FLAGS.fixAltitude)
    {
        *(old_datas.altPtr-1) = dataPacket.altitude;
        dataPacket.altitude = dataPacket.altitude -2;
        *old_datas.altPtr = dataPacket.altitude;
        //setNewStatus();
    }
    else if (!controlVar.FLAGS.manupulationFalling && !controlVar.FLAGS.fixAltitude)
    {
        if (dataPacket.altitude > 798 && dataPacket.altitude < 802)
        {
            controlVar.FLAGS.manupulationFalling = TRUE;
        }
        *(old_datas.altPtr-1) = dataPacket.altitude;
        dataPacket.altitude = dataPacket.altitude+2;
        *old_datas.altPtr = dataPacket.altitude;
        //setNewStatus();
    }
    setNewStatus();
    // dataPacket.altitude = dataPacket.altitude+1;
    // *old_datas.altPtr = dataPacket.altitude;
    // setNewStatus();

}
void Communucation::setNewStatus(void)
{
    if (package_number == 1 || (dataPacket.altitude >= 0 &&  dataPacket.altitude < 3) )
    {
        // ilk açıldığında ve yükseklik   =   0 <= yükseklik < 3
        dataPacket.FLIGHT_STATUS  = STAT_WAITING;
    }

    else if (package_number != 1 && dataPacket.altitude > *(old_datas.altPtr-1) && dataPacket.altitude  >= 4)
    {
        // Şimdikik yükseklik  > önceki yükseklik ve  yükselik >= 4
        dataPacket.FLIGHT_STATUS  = STAT_RISING;
    }

    else if (package_number != 1 &&  403 > dataPacket.altitude && 397 < dataPacket.altitude && !controlVar.FLAGS.seperatedBefore)
    {
        // 395<yükseklik<405 ve dahaönceAyrışmadıYSA
        // Ayrışma Mekanizmasını Devreye Sok.
        dataPacket.FLIGHT_STATUS            = STAT_SEPERATING    ;
        controlVar.FLAGS.seperatedBefore    = TRUE               ;

    }

    else if (package_number != 1 && dataPacket.altitude <*(old_datas.altPtr-1) && controlVar.FLAGS.seperatedBefore && dataPacket.altitude >= 190 && dataPacket.altitude <= 210 && !controlVar.FLAGS.fixAltitudeBefore)
    {
        //      190 < x < 210 // 185
        Serial.println("NOW ALTITUDE IS FIXING!!!!!!!!*********");
        controlVar.FLAGS.fixAltitude        = TRUE           ;
        controlVar.FLAGS.fixAltitudeBefore  = TRUE           ;
        dataPacket.FLIGHT_STATUS            = STAT_FIXEDALT  ;
        // Burada hemen motora güç ver çünkü diğer yerlere gidene kadar time elapsed olacak.
    }

    else if (package_number != 1 && dataPacket.altitude < *(old_datas.altPtr-1) && controlVar.FLAGS.seperatedBefore)
    {
        dataPacket.FLIGHT_STATUS  =  STAT_PAYFALL;
    }

    else if (package_number != 1 && controlVar.FLAGS.seperatedBefore && dataPacket.altitude >= 0  && dataPacket.altitude < 5)
    {
        // AYrıştıysa ve  0<= Yükseklik < 5
        // strcpy(data.FLIGHT_STATUS,"RESCUE");
        dataPacket.FLIGHT_STATUS  =  STAT_RESCUE;
    }

    else if (package_number != 1 && dataPacket.altitude < *(old_datas.altPtr-1))
    {
        // Şimdiki Yükseklik < Önceki Yükseklik.
        // strcpy(data.FLIGHT_STATUS,"FLIGHTFALL");
        dataPacket.FLIGHT_STATUS  =  STAT_FLIGHTFALL;
    }
}
void Communucation::readTemperature(void)
{
//   dataPacket.temperature = (float)sensors.bmpData.temperature; // An example read from sensor, beware that this just reads the existing data and a flush before a read is necessary to get new data.
    // dataPacket.temperature = dataPacket.temperature +1;
    *old_datas.tempPtr = dataPacket.temperature;
}
void Communucation::readTurnNumber(void)
{
    // dataPacket.turn_number = dataPacket.turn_number+1;
    *old_datas.turnPtr = dataPacket.turn_number;
}

void Communucation::readPitch(void)
{
    // dataPacket.pitch = dataPacket.pitch+1;
    *old_datas.pitchPtr = dataPacket.pitch;
}

void Communucation::readRoll(void)
{
    // dataPacket.roll = dataPacket.roll+1;    
    *old_datas.rollPtr = dataPacket.roll;
}

void Communucation::readYaw(void)
{
    // dataPacket.yaw = dataPacket.yaw+1;
    *old_datas.yawPtr = dataPacket.yaw;
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
            ACKPacket.ACKType   = ACKType_NONE  ;
            ACKPacket.ACK       = ACK_SUCCESS   ;
            sendTelemetries(); // SEND TELEMETRIES AGAIN.
            break;
        case NOTHING_MISSED_H:
            ACKPacket.ACKType   = ACKType_NONE  ;
            ACKPacket.ACK       = ACK_NONE      ;
            break; // DO NOTHING
        case VIDEO_SIZE_H:
            // Serial.print("V_S Buffer : ");
            // Serial.println(gcsPacket.bufferArray);
            if (!controlVar.FLAGS.videoSizeTransferred)
            {
                VIDEO_SIZE = strtoul((const char*)gcsPacket.bufferArray, NULL, 10); // Video BinarySize to Unsigned Long
                // strcpy(SendingStringBuffer, "VS 1\n");
                ACKPacket.ACKType                       = ACKType_VS  ;
                ACKPacket.ACK                           = ACK_SUCCESS ;
                controlVar.FLAGS.videoSizeTransferred   = TRANSFER_COMPLETED        ;
                // Serial.print("Video Size Reached.. ");
                //Serial.println(VIDEO_SIZE);
            }
            break;

        case VIDEO_DATA_H:

            if (!controlVar.FLAGS.videoTransferCompleted && controlVar.FLAGS.videoSizeTransferred)
            {
                REACHED_SIZE += (int)strlen((const char*)gcsPacket.bufferArray);
                //storage.writeVideo(SD,gcsPacket.bufferArray);
                ACKPacket.ACKType = ACKType_VID;
                if (REACHED_SIZE >= VIDEO_SIZE)
                {
                    ACKPacket.ACK = ACK_VID_COMP;
                    controlVar.FLAGS.videoTransferCompleted = TRANSFER_COMPLETED;
                }
                else
                {
                    ACKPacket.ACK = ACK_SUCCESS;
                }
            }
            else
            {
                ACKPacket.ACK = ACK_VID_COMP;
            }
            break;  
        case ERROR_H : 
            ACKPacket.ACKType   = ACKType_NONE ;
            ACKPacket.ACK       = ACK_UNSUCCESS;
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


    //if (Serial.availableForWrite() >= (uint8_t)sizeof(dataPacket))
    Serial.write((const uint8_t * )&dataPacket,sizeof(dataPacket));
}

void Communucation::sendACK(void)
{
     // send InformationFrame   
     // send ACKFrame

    // if (Serial.availableForWrite() >= (uint8_t)sizeof(ACKPacket))
    Serial.write((const uint8_t * )&ACKPacket,sizeof(ACKPacket));

}


void Communucation::manualServiceCheck(void)
{
    
    if ( 88 == COMMAND ) // Manuel Ayrışma...
    {
        releasePayload();
    }
    else if ( 77 == COMMAND  ) // Manuel Tahrik (MOTOR) for [TESTING] PERVANE
    {
        manualmotorActivation(true);
    }
    else if ( 99 == COMMAND ) // NOT TESTING JUST MANUAL TAHRIK ALL THE TIME
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
        // bool tenSecond = false;
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

        dataPacket.altitude         = 32.11 ;
        dataPacket.pressure         = 11.333;
        dataPacket.temperature      = 15    ;
        dataPacket.turn_number      = 1     ;
        dataPacket.pitch            = 7.11  ;
        dataPacket.yaw              = -2.11 ;
        dataPacket.roll             = 4.11  ;
        dataPacket.batteryVoltage   = 3.33  ;
        dataPacket.descentSpeed     = 0     ;
        dataPacket.TEAM_ID          = 1152  ;
        // dataPacket.package_number = 4;
        dataPacket.latitude         = 2     ;
        dataPacket.longtitude       = 5     ;
        
        
        afterReading = millis(); 
        RemainTime   = afterReading - beforeReading;
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
            }
        }
        ACKPacket.ACKType = ACKType_END; // Just make ACK Type 2 (END SIGNAL) 
        ACKPacket.ACK     = ACK_END_SIGNAL;
        sendACK();
        
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
        controlVar.FLAGS.startReaded = 1;
        STARTS_BUF[0] = 5;
        int LenBuff = Serial.readBytes(STARTS_BUF, 3);
        if (LenBuff > 0)
        {
            controlVar.FLAGS.startReaded = TRUE;
        }
        if (controlVar.FLAGS.startReaded)
        {
            if (STARTS_BUF[0] == 5) // STARTS COMMAND..
            {
                // systemActivated = true;
                controlVar.FLAGS.systemActivated = TRUE;
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
            // Set Motor Speed To Normal IN RIGHT HERE!!.
        }
    }
}


void Communucation::getProtocolStatus(void)
{

    switch (Buffer[0])
    {
        case NOTHING_MISSED_H:
            HEADER          = NOTHING_MISSED_H  ;
            MAX_GCS_BYTES   = GCS_Pckt_Normal   ;
            // memcpy(&gcsPacket,  Buffer , 2); // BU BURDA OLMAMALI
            break;
        case MISSED_DATA_AV_H:
            HEADER          = MISSED_DATA_AV_H  ;
            MAX_GCS_BYTES   = GCS_Pckt_Normal   ;
            // memcpy(&gcsPacket,  Buffer , 2); // BU BURDA OLMAMALI
            break;
        case VIDEO_SIZE_H:
            HEADER          = VIDEO_SIZE_H      ;
            MAX_GCS_BYTES   = GCS_Pcket_VIDEO   ;
            // memcpy(&gcsPacket,  Buffer , sizeof(gcsPacket)); // BU BURDA OLMAMALI
            break;
        case VIDEO_DATA_H:
            HEADER          = VIDEO_DATA_H    ;
            MAX_GCS_BYTES   = GCS_Pcket_VIDEO ;
            // memcpy(&gcsPacket,  Buffer , sizeof(gcsPacket)); // BU BURDA OLMAMALI
            break;    
        default:
            HEADER  = ERROR_H;
            break;
    }
}
void Communucation::stringCopies(void)
{

    dataPacket.FrameType               = DataFrameHeader; // Says its DataFrame  Normally 0 I Changed // 0xBB 
    ACKPacket.FrameType                = ACKFrameHeader; // ACK Frame // Normally 1 I changed to // 0xCC
    gcsPacket.bufferArray[0]           = '\0';
    dataPacket.FLIGHT_STATUS           = STAT_FLIGHTFALL; 

    dataPacket.VIDEO_TRANSMISSION_INFO = TRANSFER_NOT_COMPLETED; 
    dataPacket.package_number          = 1;
    
    ACKPacket.ACKType     = ACKType_NONE ;       // First None
    ACKPacket.ACK         = ACK_NONE     ;      //first none
    controlVar.resetFlag  = RESET_FLAGS  ;
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
    // Serial.printf("%.2f",sensors.mpuData.yaw);Serial.printf(",");Serial.printf("%.2f", dataPacket.turn_number);Serial.printf(",");
    // Serial.printf("%s",data.VIDEO_TRANSMISSION_INFO);
    // Serial.printf(">\n");


    
}

void Communucation::saveTelemetries(void){
  // Some magic badass telemetry constructor. I now re-evalute my career choices.
//   sprintf(telemetryBuffer,"<%d,%d,%02d/%02d/%02d %02d:%02d:%02d,%.2f,%.2f,%.2f,%.2f,%.1f,%.2f,%.1f,%.2f,%s,%.2f,%.2f,%.2f,%.2f,%s>",
//   TEAM_ID,package_number,sensors.gpsData.day,sensors.gpsData.month,sensors.gpsData.year,sensors.gpsData.hour,sensors.gpsData.minute,sensors.gpsData.second,
//   sensors.bmpData.pressure,sensors.bmpData.altitude,abs(sensors.prevAltitude - sensors.bmpData.altitude),sensors.bmpData.temperature,data.batteryVoltage,
//   sensors.gpsData.latitude,sensors.gpsData.longitude,sensors.gpsData.altitude,data.FLIGHT_STATUS,sensors.mpuData.pitch,sensors.mpuData.roll,sensors.mpuData.yaw,
//   dataPacket.turn_number,data.VIDEO_TRANSMISSION_INFO
//   );
//   storage.writeTelemetry(SD,telemetryBuffer);
}
