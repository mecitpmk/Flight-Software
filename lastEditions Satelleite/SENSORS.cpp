#include "SENSORS.h"

SENSORS::SENSORS()
{

    bmpData = {0};
    mpuData = {0};
    gpsData = {0};

}

void SENSORS::initAllSensor()
{

    bmp.init();
    bmp.setSensorInNormalMode(BMP3_NO_OVERSAMPLING,BMP3_OVERSAMPLING_8X,BMP3_IIR_FILTER_COEFF_3, BMP3_ODR_50_HZ,false);
                                /*  Or we can use x16 Pressure , x2 Temperature IIR FILTER 4X , 25 HZ
                                    BMP3_OVERSAMPLING_2X ,  BMP3_OVERSAMPLING_16X  , BMP3_IIR_FILTER_COEFF_15  , BMP3_ODR_25_HZ  */
    // Initalize MPU
    
    WIRE_PORT.begin();
    WIRE_PORT.setClock(400000);
    myICM.begin(WIRE_PORT, AD0_VAL);
    Serial.println(myICM.statusString());
    Serial2.begin(9600,SERIAL_8N1, 15, 12 ); //Start GPS Software Serial
//     while (!Serial2) {
//     ; // wait for serial port to connect. Needed for native USB
//   }
    bool success = true;

    success &= (myICM.initializeDMP() == ICM_20948_Stat_Ok);

   

    // Enable the DMP Game Rotation Vector sensor
    success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR) == ICM_20948_Stat_Ok);

    
    success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat6, 0) == ICM_20948_Stat_Ok); // Set to the maximum


    // Enable the FIFO
    success &= (myICM.enableFIFO() == ICM_20948_Stat_Ok);

    // Enable the DMP
    success &= (myICM.enableDMP() == ICM_20948_Stat_Ok);

    // Reset DMP
    success &= (myICM.resetDMP() == ICM_20948_Stat_Ok);

    // Reset FIFO
    success &= (myICM.resetFIFO() == ICM_20948_Stat_Ok);

    if (success)
    {
        Serial.println("DMP ENABLED...");
    }
    else
    {
        Serial.println("DMP nottttttt...");
    }
}   

void SENSORS::flushBMPData(void)
{
    struct bmp_data sensorData;

    if(bmp.getSensorData(&sensorData, true)){ // Read data into the struct, and pass that data to global storage.
        bmpData.pressure = (float) sensorData.pressure;
        prevAltitude = bmpData.altitude;
        bmpData.altitude = (float) sensorData.altitude;
        bmpData.temperature = (float) sensorData.temperature;
    }else {
        Serial.println("SOMEHING WRONG IN BMP!!");
        // Sensor error occurred! Maybe skip or just give the old data? or raise a error flag?
    }
}

void SENSORS::flushMPUData(void)
{

    icm_20948_DMP_data_t data;
    myICM.readDMPdataFromFIFO(&data);
    while (true)
    {
        if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
        {
            

            if ((data.header & DMP_header_bitmap_Quat6) > 0) // We have asked for GRV data so we should receive Quat6
            {
               
                // Scale to +/- 1
                double q1 = ((double)data.Quat6.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
                double q2 = ((double)data.Quat6.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
                double q3 = ((double)data.Quat6.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30

                             // Convert the quaternions to Euler angles (roll, pitch, yaw)
                // https://en.wikipedia.org/w/index.php?title=Conversion_between_quaternions_and_Euler_angles&section=8#Source_code_2

                double q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

                double q2sqr = q2 * q2;

                // roll (x-axis rotation)
                double t0 = +2.0 * (q0 * q1 + q2 * q3);
                double t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
                double roll = atan2(t0, t1) * 180.0 / PI;

                // pitch (y-axis rotation)
                double t2 = +2.0 * (q0 * q2 - q3 * q1);
                t2 = t2 > 1.0 ? 1.0 : t2;
                t2 = t2 < -1.0 ? -1.0 : t2;
                double pitch = asin(t2) * 180.0 / PI;

                // yaw (z-axis rotation)
                double t3 = +2.0 * (q0 * q3 + q1 * q2);
                double t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
                double yaw = atan2(t3, t4) * 180.0 / PI;
                mpuData.roll  = (float)roll;
                mpuData.pitch = (float)pitch;
                mpuData.yaw   = (float)yaw;

            }
        }
        else
        {
            delay(20);
            continue;
        }
        break;
    }


    
}

void SENSORS::flushGPSData(void)
{ 
    
    while (Serial2.available() > 0)
    {
        if (gps.encode(Serial2.read()))
        {
            if (gps.location.isValid())
            {
                gpsData.latitude = gps.location.lat();
                gpsData.longitude = gps.location.lng();
            }
            else
            {
                gpsData.latitude  = 0.0;
                gpsData.longitude = 0.0;
            }
            if (gps.altitude.isValid())
            {
                gpsData.altitude = gps.altitude.meters();
            }
            else
            {
                gpsData.altitude = -1;
            }

            if(!gps.date.isValid()){
                gpsData.month = gps.date.month();
                gpsData.day = gps.date.day();  
                gpsData.year = gps.date.year();

                gpsData.hour = gps.time.hour();
                gpsData.minute = gps.time.minute();
                gpsData.second = gps.time.second();
            }
            
        }
    }
    

}
