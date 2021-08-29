#include "STORAGE.h"

#define TELEMETRY_FILE "/telemetry.txt"
#define VIDEO_FILE "/video.mp4"

STORAGE::STORAGE()
{
  isTelemOpen = false;
  isVidOpen = false;
}

void STORAGE::initStorage(void){
  
  if(!SD.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
   Serial.println("No SD card attached");
   return;
  }
}

void STORAGE::writeTelemetry(fs::FS &fs,char* data){
  if(!isTelemOpen){
    telemFile = fs.open(TELEMETRY_FILE,FILE_APPEND); //File mode FILE_WRITE or FILE_APPEND. Use append for telemetry. Write for video.
    if(telemFile) isTelemOpen = true;
    else Serial.println("Failed to open telemetry file.");
 }
  if(isTelemOpen) telemFile.print(data);
  closeTelemetry();
}

void STORAGE::closeTelemetry(){
 if(isTelemOpen){
  telemFile.close();
  isTelemOpen = false;
 }
}

void STORAGE::writeVideo(fs::FS &fs,char* data){
    if(!isVidOpen){
    vidFile = fs.open(VIDEO_FILE,FILE_WRITE); //File mode FILE_WRITE or FILE_APPEND. Use append for telemetry. Write for video.
    if(vidFile) isTelemOpen = true;
    else Serial.println("Failed to open telemetry file.");
 }
  if(isVidOpen) vidFile.print(data);
  closeVideo();
}

void STORAGE::closeVideo(){
 if(isVidOpen){
  vidFile.close();
  isVidOpen = false;
 }
}
