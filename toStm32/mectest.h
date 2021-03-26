
#ifndef MECTEST_H
#define MECTEST_H
#include <Arduino.h>

class DATACLASS
{
    public:
        /*const char *FLIGHT_RISING = "RISING";
        const char *FLIGHT_WAITING = "WAITING";
        const char *FLIGHT_SEPERATING = "SEPERATING"; 
        const char *FLIGHT_FALLING = "FALLING";
        const char *PAYLOAD_FALLING = "PAYLFALL";
        const char *PAYLOAD_RESCUE = "RESCUE";
        const char *PAYLOAD_FIXED_ALT = "ALTFIXED";*/

        // made them again const char to prevent heap memory overflow..
        String FLIGHT_RISING = "RISING";
        String FLIGHT_WAITING = "WAITING";
        String FLIGHT_SEPERATING = "SEPERATING";
        String FLIGHT_FALLING = "FALLING";
        String PAYLOAD_FALLING = "PAYLFALL";
        String PAYLOAD_RESCUE = "RESCUE";
        String PAYLOAD_FIXED_ALT = "ALTFIXED";

        //char *SatStatus[6] ={"WAITING","RISING","SEPERATING","FALLING","PAYLFALL","RESCUE"}; // DELETE YAPABILIRSIN.
        //char *SatelleiteStatus = FLIGHT_WAITING;
        String *SatelleiteStatus = &FLIGHT_WAITING;
        //const char numbersChar[10][2] = {"1 ","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 ","10 "};    
        float altitude = 15;   // Yukseklik
        float pressure = 0;   //Basınç
        float temperature = 0; //Sıcaklık
        float turn_number = 0; // Dönüş Sayısı
        float pitch = 0; // Pitch
        float yaw = 0; // Yaw
        float roll = 0; // Roll
        //float *flDatas[7] = {&altitude,&temperature,&pressure,&turn_number,
        //                            &pitch,&yaw,&roll};  // Data'ların adress'lerini tutan array
        //                                                //Gonderilemeyen Datalarda işimize yarayacak. Dursun.
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
        const uint8_t TEAM_ID = 111; // Takım Numarası
        int transmittedFeedBack; // Bununla alakalı dahaden işlem yapmadım. Ground Station'dan veri alım sırasında kullanacağız.
        uint8_t feedbackChar; 
        OLDDATACLASS old_datas =  OLDDATACLASS(); // Eski dataları tuttuğumuz class.
        DATACLASS data = DATACLASS();           // Normla dataları tuttuğumuz class.
        //DATACLASS *data = &datas; // Bunu bilerek yaptım -> ları . ya dönüştürmekten vakit kaybetmemek için :D 
        unsigned long elapsed_time ;
        // const unsigned long interval = 3000; Normally 800 falan olmalı1!!
        const uint16_t interval = 500;
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
        uint8_t serviceVariable = 0;
        bool didService = false;
        bool serviceRecall = false;
        bool systemActivated= false;
        bool videoByteTakedBefore = false;
        
        int totalVideoBytes = 0;
        
        char incomingDataChar = '0';
        
        
        
        
        bool serialReadCompleted = false;

        //int serviceReqidx = 0;
        //const static int SVRQ_SIZE = 10;
        //int serviceRequests[SVRQ_SIZE];

        unsigned long reSendPeriod = 1;
        bool reSendComp = false;
        bool serialAv = false;
        bool videoEnabled = false;
        
        uint16_t bufferIndex = 0;

        char* serialBuffer = new char[100];

        //uint16_t ReachedByte = 0;
        unsigned long ReachedByte = 0;
        
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
        void sendOnebyOneLine();
        uint8_t waitforResponse(char *receivedBuffer);
        void catchingMissedData();
        void releasePayload();
        void manualServiceCheck(bool fromOutside);
        void manualmotorActivation(bool fortesting);
        void getDatas();
        char* subStr (char* str, char *delim, const uint8_t index);
        void testingSerial();
        void fromString(const char& str);
        void cleanInputBuffer();
        
        

}; 
#endif