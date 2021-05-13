
#ifndef MAVLINKPROTOCOL_H
#define MAVLINKPROTOCOL_H
#include <Arduino.h>

class DATACLASS
{
    public:
       
        char FLIGHT_STATUS[20] = "WAITING";
        char VIDEO_TRANSMISSION_INFO[5] = "HAYIR";

        float altitude = 15;   // Yukseklik
        float pressure = 0;   //Basınç
        float temperature = 0; //Sıcaklık
        float turn_number = 0; // Dönüş Sayısı
        float pitch = 0; // Pitch
        float yaw = 0; // Yaw
        float roll = 0; // Roll
 
        DATACLASS(){};
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

        float *altPtr = &altitude[1]; // En son arrayın kaçıncı indexinde kaldığımızı bulmaya yarayacak.
        float *presPtr = pressure;
        float *tempPtr = temperature;
        float *turnPtr = turn_number;
        float *pitchPtr = pitch;
        float *yawPtr = yaw;
        float *rollPtr = roll;
        OLDDATACLASS(){};
};


class Communucation
{
    public:
    
        uint16_t package_number = 1; // Paket Numarası Normal counter gibi..
        const uint16_t TEAM_ID = 29779; // Takım Numarası


        OLDDATACLASS old_datas =  OLDDATACLASS(); // Eski dataları tuttuğumuz class.
        DATACLASS data = DATACLASS();           // Normla dataları tuttuğumuz class.
        //DATACLASS *data = &datas; // Bunu bilerek yaptım -> ları . ya dönüştürmekten vakit kaybetmemek için :D 
        
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
        
        uint8_t COMMAND = 0;
        bool didService = false;
        bool serviceRecall = false;
        bool systemActivated= false;

        bool videoByteTakedBefore = false;
            
        uint8_t bufferIndex = 0;

        const static uint8_t BUF_SIZE = 150;
        char Buffer[BUF_SIZE];
        
        const char StartMark = '<';
        const char EndMark = '>';
        bool Readed = false;
        bool isReading = false;

        //uint16_t ReachedByte = 0;
        unsigned long REACHED_SIZE = 0;
        unsigned long VIDEO_SIZE = 0;
        bool videoTransferCompleted = false;
        char *CURRENT_VIDEO_BIN = NULL;

        unsigned long beforeReading;
        unsigned long afterReading;
        unsigned long RemainTime;
        
        

        Communucation();                    //constructor.
    
        void readPressure();    //Basıncı okuma fonksiyonu
        void readAltitude();    //Yüksekliği okuma fonksiyonu
        void readTemperature(); //Temp okuma fonk.
        void readTurnNumber(); // Dönüş Sayısı okuma fonk.
        void mainLp(); // Tek tek fonksiyonları çalıştırıp dataları auto gönderiyor.
        void readPitch(); // Pitch'i oku
        void readRoll(); // Roll'u oku
        void readYaw(); // Yaw'u oku.
        void setNewStatus();
        bool waitforResponse();
        void releasePayload();
        void manualServiceCheck();
        void manualmotorActivation(bool fortesting);
        void getDatas();
        char* subStr (char* str, char *delim, const uint8_t index);
        void sendTelemetries();


        
        

}; 
#endif