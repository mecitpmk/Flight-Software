
#ifndef MAVLINKPROTOCOL_H
#define MAVLINKPROTOCOL_H
#include <Arduino.h>
#include <string.h>
class DATACLASS
{
    public:
       
        
        
        
        //char FLIGHT_STATUS[21]  = "WAITING"; // STM32 DOESNT ALLOW IT ????
        //char VIDEO_TRANSMISSION_INFO[6] = "HAYIR";  // STM32 DOESNT ALLOW IT ????
        //
        char FLIGHT_STATUS[21];
        char VIDEO_TRANSMISSION_INFO[6];
        


        float altitude;   // Yukseklik
        float pressure ;   //Basınç
        float temperature ; //Sıcaklık
        float turn_number ; // Dönüş Sayısı
        float pitch ; // Pitch
        float yaw ; // Yaw
        float roll; // Roll
 
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
        
        uint16_t COMMAND = 0;
        bool didService = false;
        bool serviceRecall = false;
        bool systemActivated= false;

        bool videoByteTakedBefore = false;
            
        uint16_t bufferIndex = 0; // uint8_t

        const static uint16_t BUF_SIZE = 280; // 85
        char Buffer[BUF_SIZE];
        // char ReserveBuffer[BUF_SIZE];
        char SendingStringBuffer[30];
        


        const char StartMark = '<';
        const char EndMark = '>';
        bool Readed = false;
        bool isReading = false;

        uint16_t INTERV ;
        char incomingDataChar;

        int LENGTH;
        char* Tag; char* CMMND;
        char* L; char* VIDEO_B;
        char* CHCK_S;
        


        //uint16_t ReachedByte = 0;
        unsigned long REACHED_SIZE = 0;
        unsigned long VIDEO_SIZE = 0;
        bool videoTransferCompleted = false;
        char *CURRENT_VIDEO_BIN = NULL;

        unsigned long beforeReading;
        unsigned long afterReading;
        unsigned long RemainTime;
        
        uint16_t CHCKSM;
        uint16_t VIDEO_BIN_LENGHT;
        

        Communucation();                    //constructor.
    
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
        void subStr (const char* str, const char* delim, const uint8_t index, char** PARAM_CHAR);
        void sendTelemetries(void);
        void clearSerialBuffer(void);

        
        

}; 
#endif