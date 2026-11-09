#include <WiFi.h>
#include <PubSubClient.h>

#include "definition.h"

const char* lcl_ssid = "rede";
const char* lcl_pass = "senha";

// ============== MAIN ==============
void setup(){
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  Serial.println("\n\nInitializing...");

  gpioBegin();
  initFileSystem();

  initWifi();

  Serial.print("\n===== Welcome to MyIot =====\n");

  
}

void loop(){

  if( gpioUpdate() ){
    sendMqtt();
  }

  mqttLoop();

  promptLoop();

  serverLoop();

  digitalWrite(2, HIGH);
}

