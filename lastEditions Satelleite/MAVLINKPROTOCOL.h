
#ifndef MAVLINKPROTOCOL_H
#define MAVLINKPROTOCOL_H
#include <Arduino.h>
#include <string.h>
#include <WiFiUdp.h>
#include "SENSORS.h"
#include "STORAGE.h"
#include <ESP32Servo.h>

#define MAX_TELEMETRY_LENGTH 2048 // 2kb is probably too much but anyways. I don't wanna fail because of this in tests

class DATACLASS
{
    public:
       
        
       
        char FLIGHT_STATUS[21];
        char VIDEO_TRANSMISSION_INFO[6];
        

        float altitude;   // Yukseklik
        float pressure ;   //Basınç
        float temperature ; //Sıcaklık
        float turn_number ; // Dönüş Sayısı
        float pitch ; // Pitch
        float yaw ; // Yaw
        float roll; // Roll
        float batteryVoltage = 3.33;
        DATACLASS();
};
class OLDDATACLASS
{
    public:
        // Bu kısımda oluşturulmuş array'ların amacı, Okunan değeri direkt olarak array'a yazmak.
        // Array'ın boyutunu 5 yaptım, normalde 1 veya 2 bile olabilirdi sıkıntı yok.
        float altitude[5];
        float pressure[5];
        float temperature[5];
        float turn_number[5];
        float pitch[5];
        float yaw[5];
        float roll[5];

        float* altPtr = &altitude[1]; // En son arrayın kaçıncı indexinde kaldığımızı bulmaya yarayacak.
        float* presPtr;
        float* tempPtr ;
        float* turnPtr ;
        float* pitchPtr ;
        float* yawPtr ;
        float* rollPtr;
        OLDDATACLASS();
};


class Communucation
{
    public:
        SENSORS sensors = SENSORS();
        STORAGE storage = STORAGE();
        uint16_t package_number = 1; // Paket Numarası Normal counter gibi..
        const uint16_t TEAM_ID = 29779; // Takım Numarası


        OLDDATACLASS old_datas =  OLDDATACLASS(); // Eski dataları tuttuğumuz class.
        DATACLASS data = DATACLASS();           // Normla dataları tuttuğumuz class.
        
        // const unsigned long interval = 3000; Normally 800 falan olmalı1!!
        const uint16_t oneHzInterval = 1000;
        unsigned long oneHZ ;



        bool manupulationFalling = false;

        bool seperatedBefore = false;
        bool releaseCommand = false;
        bool manualMotorActive = false;
        bool fixAltitude = false;
        bool fixAltTrueBefore = false;
        const uint16_t testMotorsInterval = 10000;
        unsigned long motorElapsedTime ; 
        
        uint16_t COMMAND = 0;
        bool systemActivated= false;


        
        char Buffer[500];
        char SendingStringBuffer[80];
        char TAG[3];
        char CMMND[3];
        char VIDEO_LNG[10];

     
        const char DELIM[1] = {' '};
        bool Readed = false;

        uint16_t INTERV ;
       

        int VID_LENGTH_CHCK_S;
        /*char* Tag; char* CMMND;
        char* L; char* VIDEO_B;
        char* CHCK_S;

        char** MY_ARRAY[4] = { &Tag, &CMMND , &CHCK_S , &CURRENT_VIDEO_BIN };*/
        


        //uint16_t ReachedByte = 0;
        unsigned long REACHED_SIZE = 0;
        unsigned long VIDEO_SIZE = 0;
        bool videoTransferCompleted = false;
        

        unsigned long beforeReading;
        unsigned long afterReading;
        unsigned long RemainTime;
        
        uint16_t CHCKSM;
        uint16_t VIDEO_BIN_LENGHT;
        const char* udpAddress = "192.168.1.100"; // 192.168.1.7 works ?
        const int udpPort = 3333;   

        
        //unsigned long testCalibration ;

        // MOTORS..
        Servo ESC; // create servo object to control the ESC
        
        const int pwmPin = 4;
        char telemetryBuffer[MAX_TELEMETRY_LENGTH];


        WiFiUDP udp;

        typedef enum
        {
            NOTHING_MISSED_H,
            MISSED_DATA_AV_H,
            VIDEO_SIZE_H,
            VIDEO_DATA_H,
            ERROR_H
        }COMING_HEADER;
        COMING_HEADER HEADER;
        
        
        Communucation();                    //constructor.
        
        void stringCopies(void); // in setup() run this function to copy.
        void readPressure(void);    //Basıncı okuma fonksiyonu
        void readAltitude(void);    //Yüksekliği okuma fonksiyonu
        void readTemperature(void); //Temp okuma fonk.
        void readTurnNumber(void); // Dönüş Sayısı okuma fonk.
        void mainLp(void); // Tek tek fonksiyonları çalıştırıp dataları auto gönderiyor.
        void readPitch(void); // Pitch'i oku
        void readRoll(void); // Roll'u oku
        void readYaw(void); // Yaw'u oku.
        void setNewStatus(void);
        bool waitforResponse(void);
        void releasePayload(void);
        void manualServiceCheck(void);
        void manualmotorActivation(bool fortesting);
        void getDatas(void);
        void subStr (void); // STR COMES FROM BUFFER ! .
        void sendTelemetries(void);
        void getProtocolStatus(void);
        void saveTelemetries(void);
        
        

}; 
#endif
