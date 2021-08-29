#include "SENSORS.h"

SENSORS::SENSORS()
{

    bmpData = {0};
    mpuData = {0};
    gpsData = {0};
    // Initialize BMP388

    // bmp.init();
    // bmp.setSensorInForcedMode(BMP3_OVERSAMPLING_16X, BMP3_OVERSAMPLING_2X, BMP3_IIR_FILTER_COEFF_3); // Default sampling settings. Might wanna change later on.
    // // Initalize MPU
    // mpu.setup(0x68/*I2C address, can be figured out with an i2c scan*/);
    // mpu.calibrateAccelGyro();
    // mpu.calibrateMag();
    // //Initialize GPS
    // // Serial2.begin(9600/*GPS Data Rate*/,SERIAL_8N1, 15 /*RX*/,12/*TX*/); //Start GPS Software Serial
    
    // // Initialize data structs with 0
    // bmpData = {0};
    // mpuData = {0};
    // gpsData = {0};
}

void SENSORS::initAllSensor()
{

    bmp.init();
    bmp.setSensorInForcedMode(BMP3_OVERSAMPLING_16X, BMP3_OVERSAMPLING_2X, BMP3_IIR_FILTER_COEFF_3); // Default sampling settings. Might wanna change later on.
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
                //Initialize GPS
                // Serial2.begin(9600/*GPS Data Rate*/,SERIAL_8N1, 15 /*RX*/,12/*TX*/); //Start GPS Software Serial
                // Initialize data structs with 0
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
    // if(mpu.update()){
    //     mpuData.roll = mpu.getRoll();
    //     mpuData.pitch = mpu.getPitch();
    //     mpuData.yaw = mpu.getYaw();
    // }else{
    //     Serial.println("SOMEHING WRONG IN mpuuuu!!");
    //     // Sensor error occurred!
    // }
    
    // while (true)
    // {
    //     if (myICM.dataReady())
    //     {
    //         myICM.getAGMT();
    //         mpuData.roll  = myICM.gyrX();
    //         mpuData.pitch = myICM.gyrY();
    //         mpuData.yaw   = myICM.gyrZ(); 
    //         break;
    //     }
    // }
    icm_20948_DMP_data_t data;
    myICM.readDMPdataFromFIFO(&data);
    while (true)
    {
        if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
        {
            //SERIAL_PORT.print(F("Received data! Header: 0x")); // Print the header in HEX so we can see what data is arriving in the FIFO
            //if ( data.header < 0x1000) SERIAL_PORT.print( "0" ); // Pad the zeros
            //if ( data.header < 0x100) SERIAL_PORT.print( "0" );
            //if ( data.header < 0x10) SERIAL_PORT.print( "0" );
            //SERIAL_PORT.println( data.header, HEX );

            if ((data.header & DMP_header_bitmap_Quat6) > 0) // We have asked for GRV data so we should receive Quat6
            {
                // Q0 value is computed from this equation: Q0^2 + Q1^2 + Q2^2 + Q3^2 = 1.
                // In case of drift, the sum will not add to 1, therefore, quaternion data need to be corrected with right bias values.
                // The quaternion data is scaled by 2^30.

                //SERIAL_PORT.printf("Quat6 data is: Q1:%ld Q2:%ld Q3:%ld\r\n", data.Quat6.Data.Q1, data.Quat6.Data.Q2, data.Quat6.Data.Q3);

                // Scale to +/- 1
                double q1 = ((double)data.Quat6.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
                double q2 = ((double)data.Quat6.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
                double q3 = ((double)data.Quat6.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30

                /*
                SERIAL_PORT.print(F("Q1:"));
                SERIAL_PORT.print(q1, 3);
                SERIAL_PORT.print(F(" Q2:"));
                SERIAL_PORT.print(q2, 3);
                SERIAL_PORT.print(F(" Q3:"));
                SERIAL_PORT.println(q3, 3);
            */

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
  // This needs to be called on the main loop IMO. This does not force any update btw. It just checks if there is new data. Naming might be misleading.
//   if(Serial2.available()){
//   char c = Serial2.read();
//   if(gps.encode(c)){ // This means there is new data available. Therefore it might be good to parse and update data.
    
//     gpsData.satCount = gps.satellites();
//     gpsData.HDOP = gps.hdop();
//     gps.f_get_position(&(gpsData.latitude),&(gpsData.longitude), &(gpsData.age));
//     gpsData.altitude = gps.f_altitude();
//     int year; // Dumped data.
//     unsigned long age; // Dumped data.
//     byte month,day,hundredths; // Dumped data, will be deleted after read.
//     gps.crack_datetime(&year,&month,&day,&(gpsData.hour),&(gpsData.minute),&(gpsData.second), &hundredths, &age);
    

//     Serial.println(gpsData.latitude);
//     Serial.println(gpsData.longitude);
//     Serial.println(gpsData.altitude);

//   }
//  } 
    while (Serial2.available() > 0)
    {
        if (gps.encode(Serial2.read()))
        {
            if (gps.location.isValid())
            {
                // Serial.print(gps.location.lat(), 6);
                // Serial.print(F(","));
                // Serial.print(gps.location.lng(), 6);
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
