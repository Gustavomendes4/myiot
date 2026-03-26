#ifndef DEFINITION_H_INCLUDED
#define DEFINITION_H_INCLUDED

#define NETWORK_LIST_LEN 10

#include "prompt.h"

typedef struct{
  char ssid[33];
  char bssid[18];
  int32_t rssi;
  int32_t channel;
  wifi_auth_mode_t authMode;
}WifiNetwork;

typedef struct{
  char ssid[33];
  char pass[41];
  bool used;
}WifiFileSaved;

typedef WifiNetwork WifiNetworkList[NETWORK_LIST_LEN];
typedef WifiFileSaved WifiSavedList[NETWORK_LIST_LEN];

const char* MY_SSID  = "MyIoT";
const char* MY_PASS  = "123123123";

enum WifiConnectResult{
  WIFI_OK,
  WIFI_WRONG_PASSWORD,
  WIFI_NO_SSID,
  WIFI_TIMEOUT
};

// ============== TEMPORARIOS ==============
// WifiConnectResult connectWifi(String ssid, String pass, String* ip = NULL, bool verb = false);
bool mqtt(bool verb = false);
void mqttLoop();

// ===================================

//

//
#endif