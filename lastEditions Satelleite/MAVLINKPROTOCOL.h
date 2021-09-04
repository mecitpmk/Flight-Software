
#ifndef MAVLINKPROTOCOL_H
#define MAVLINKPROTOCOL_H
#include <Arduino.h>
#include <string.h>
// #include <WiFiUdp.h>
// #include "SENSORS.h"
// #include "STORAGE.h"
// #include <ESP32Servo.h>

#define MAX_TELEMETRY_LENGTH 2 // 2kb is probably too much but anyways. I don't wanna fail because of this in tests

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
        // SENSORS sensors = SENSORS();
        // STORAGE storage = STORAGE();
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

     
        const char DELIM[1] = {' '};
        bool Readed = false;

        uint16_t INTERV ;
       

        int VID_LENGTH_CHCK_S;



        // Before sending any dataFrame or ACK frame 
        // This frame should be send.
        
        struct GCSFrame
        {
            uint8_t tagType;
            /*
                M 88 5 11111
                0 -> N
                1 -> M
                2 -> VS
                3 -> V
            */
            uint8_t command ;
           /*
                88..
                99..
                77..
           */

            uint8_t bufferSize;      // For example video data is 11111 Buffer size will be = 5
            
            uint8_t bufferArray[500]; // Max 500 Byte!
        };

        /*
            Ground Station'dan datayı write ederken,
                503 byte basmamak için
                    Eger video datası yok ise
                    write( .... sizeof(Struct)) vermek yerine
                    write ( .... 3 ) ver [3 tane uint8_t olduğu icin 3 byte]
                    böylelikle yalnızca 3 byte göndermiş oluruz
                    Serial'i yormayız..                    


        */

        // Data and Interval should be sended from here.
        struct dataFrame
        {
            uint8_t FrameType; // Says ACK or DataFrame
            uint8_t FLIGHT_STATUS; // send Coded info? 
                        // 0 is WAITING
                        // 1 is RISING 
                        // 2    SEPERATING
                        // 3    FLIGHTFALL
                        // 4    PAYFALL
                        // 5    FIXEDALT
                        // 6    RESCUE

            uint8_t VIDEO_TRANSMISSION_INFO;
                        // 0 is No
                        // 1 is Yes
            

            float altitude          ;   // Yukseklik
            float pressure          ;   //Basınç
            float temperature       ; //Sıcaklık

            float turn_number       ; // Dönüş Sayısı
            float pitch             ; // Pitch
            float yaw               ; // Yaw
            float roll              ; // Roll

            float batteryVoltage    ;
            float descentSpeed      ;

            uint16_t Interval       ;
            uint16_t TEAM_ID  ;
            uint16_t package_number ;

            float latitude          ;
            float longtitude        ;

            // Add time infos..
            
            
        } __attribute__((packed)) ;
        
        /*
            Read first byte(first 8 bits) from GCS and
                make decision which info is comed..
                    Meaning  : GCS Check its ACK or dataFrame from first byte.
                    Ex : if      bytes[0] == 0 its Dataframe memcpy to Dataframe structure.
                         else if bytes[0] == 1 its ACKFrame  mempcy to ackFrame structure.
        */


        // When the GCS waits respond from us
        // Send that frame
        struct ACKFrame
        {
            uint8_t FrameType;
            uint8_t ACKType;
            uint8_t ACK;
            /*

                ACKType:
                    0 -> VS   ACK
                    1 -> V    ACK
                    2 -> E    ACK (COMM ENDED)
                ACK :
                    0 -> UNSUCCESSFULLY
                    1 -> SUCCESSFULLY
                    2 -> if ACKType is  1 (V), Make ACK 2. Means Completed.
            */
        };
        
        // Search for default parameeter for struct
            // .FrameType = 0 gibi.
        // struct dataFrame dataPacket ;
        // struct ACKFrame ACKPacket   ;
        // struct GCSFrame gcsPacket   ;

        // dataPacket.FrameType     = 0; // Says its DataFrame 
        // ACKPacket.FrameType      = 1; // ACK Frame
        // gcsPacket.bufferArray[0] = '\0';
        
        //uint16_t ReachedByte = 0;
        unsigned long REACHED_SIZE = 0;
        unsigned long VIDEO_SIZE = 0;
        bool videoTransferCompleted = false;
        

        unsigned long beforeReading;
        unsigned long afterReading;
        unsigned long RemainTime;
        
        uint16_t CHCKSM;
        uint16_t VIDEO_BIN_LENGHT;
        // const char* udpAddress = "192.168.1.100"; // 192.168.1.7 works ?
        // const int udpPort = 3333;   

        
        //unsigned long testCalibration ;

        // MOTORS..
        // Servo ESC; // create servo object to control the ESC
        struct dataFrame dataPacket ;
        struct ACKFrame ACKPacket   ;
        struct GCSFrame gcsPacket   ;
        
        const int pwmPin = 4;
        char telemetryBuffer[MAX_TELEMETRY_LENGTH];


        // WiFiUDP udp;
        typedef enum
        {
            NOTHING_MISSED_H    = 0,
            MISSED_DATA_AV_H    = 1,
            VIDEO_SIZE_H        = 2,
            VIDEO_DATA_H        = 3,
            ERROR_H             = 4
        }COMING_HEADER;
        COMING_HEADER HEADER;
        
        
        Communucation();                    //constructor.
        
        void sendPackage(void) ; // First send 
        void sendACK(void);
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
        // void subStr (void); // STR COMES FROM BUFFER ! .
        void sendTelemetries(void);
        void getProtocolStatus(void);
        void saveTelemetries(void);
        
        

}; 
#endif
