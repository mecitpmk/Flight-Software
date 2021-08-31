#include "MAVLINKPROTOCOL.h"
#include <WiFi.h>

Communucation c;
const char* networkName = "SUPERONLINE-WiFi_6668";
const char* networkPswd = "A4WH9UATYMAU";
boolean connectedW = false;


void setup() {
  Serial.begin(115200);
  c.stringCopies();
  connectToWiFi(networkName, networkPswd);
  c.sensors.initAllSensor();
  c.storage.initStorage();

}

void loop() {
  if (connectedW)
  {
    c.mainLp();
  }

}

void connectToWiFi(const char* ssid, const char* pwd) {
    Serial.println("Connecting to WiFi network: " + String(ssid));

    // delete old config
    WiFi.disconnect(true);
    //register event handler
    WiFi.onEvent(WiFiEvent);

    //Initiate connection
    WiFi.begin(ssid, pwd);

    Serial.println("Waiting for WIFI connection...");
}

//wifi event ha ndler
void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        //When connected set 
        Serial.print("WiFi connected! IP address: ");
        Serial.println(WiFi.localIP());
        //initializes the UDP state
        //This initializes the transfer buffer
        c.udp.begin(WiFi.localIP(), c.udpPort);
        connectedW = true;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        connectedW = false;
        break;
    default: break;
    }
}
