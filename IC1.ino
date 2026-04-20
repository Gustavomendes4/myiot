#include <WiFi.h>
#include <PubSubClient.h>

#include "definition.h"

// ============== MAIN ==============
void setup(){
  Serial.begin(115200);

  Serial.println("\n\nInitializing...");
  gpioBegin();
  initFileSystem();
  initWifi();

  Serial.print("\n===== Welcome to MyIot =====\n");
}

void loop(){
  gpioUpdate();

  // mqttLoop();

  promptLoop();

  serverLoop();
}

