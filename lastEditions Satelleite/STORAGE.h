#ifndef STORAGE_H
#define STORAGE_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Handles SD as well as Flash
class STORAGE
{
private:
    File telemFile;
    File vidFile;
    bool isTelemOpen;
    bool isVidOpen;

public:
    
    STORAGE();
    void initStorage(void);
    void writeTelemetry(fs::FS &fs,char* data);
    void closeTelemetry(void);
    void writeVideo(fs::FS &fs,char* data);
    void closeVideo(void);
};

#endif
