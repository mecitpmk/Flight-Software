#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <bmp_i2c.h>
#include <TinyGPS++.h>
#include "ICM_20948.h"


#define WIRE_PORT Wire // Your desired Wire port.      Used when "USE_SPI" is not defineda
#define AD0_VAL 1      // The value of the last bit of the I2C address.                

class SENSORS
{
private:
    BMP3_I2C bmp{BMP3_I2C(0x77)};
    ICM_20948_I2C myICM; // Otherwise create an ICM_20948_I2C object
    TinyGPSPlus gps;

public:
    struct {
        float pressure;
        float altitude;
        float temperature;
    } bmpData;

    float prevAltitude;

    struct {
        float roll;
        float pitch;
        float yaw;
    } mpuData;

    struct {
        //unsigned long int satCount;
        //unsigned long int HDOP;
        float latitude;
        float longitude;
        unsigned long age; // Idk if this is useful but added anyways.
        float altitude;
        int year;
        byte month,day,hour,minute,second;
        } gpsData;
    SENSORS();
    void initAllSensor(void);
    void flushBMPData(void); // Call this to update sensor datas
    void flushMPUData(void);
    void flushGPSData(void);
};

#endif
