#pragma once
#include <inttypes.h>




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
    const uint16_t TEAM_ID  ;
    uint16_t package_number ;

    float latitude          ;
    float longtitude        ;

    // Add time infos..
    
    
} __attribute__((packed)) ;

struct GCSFrame
{
    uint8_t tagType;
    /*
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
