#include <WiFi.h>
#include "definition.h"

const int wifi_tries = 20;
bool WIFIconnected = false;


WifiConnectResult connectWifi(String ssid, String pass, String* ip = NULL, bool verb = false){

  WiFi.disconnect(true);  
  WiFi.mode(WIFI_MODE_APSTA);
  delay(200);

  WiFi.begin(ssid, pass.c_str());

  if(verb){
    Serial.print("\nConectando à rede: ");
    Serial.print(ssid);
    Serial.print(" | Senha: ");
    Serial.print(pass);
  }

  int i = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
      
    if(i++ == wifi_tries){
      if(verb)
        Serial.println("Erro ao conectar à rede Wifi");
      
      WIFIconnected = false;  
      break;
    }

    if(verb)
      Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED){

    if(verb){
      Serial.print("\nConectado:\n\tRede: ");
      Serial.print(ssid);
      Serial.print("\n\tIP: ");
      Serial.println(WiFi.localIP());
    }

    WIFIconnected = true;

    if(ip){
      *ip = WiFi.localIP().toString();
    }

    return WIFI_OK;
  }

  wl_status_t status = WiFi.status();
  WiFi.disconnect(false);

  if(status == WL_CONNECT_FAILED){
    return WIFI_WRONG_PASSWORD;

  }else if(status == WL_NO_SSID_AVAIL){
    return WIFI_NO_SSID;

  }else{
    return WIFI_TIMEOUT;
  }  
}

int scanWifiNetworks(WifiNetwork list[]){

  int len = WiFi.scanNetworks();
  len = min(len, NETWORK_LIST_LEN);
  
  for (int i = 0; i < len; i++){
    strncpy(list[i].ssid, WiFi.SSID(i).c_str(), sizeof(list[i].ssid) - 1);
    strncpy(list[i].bssid, WiFi.BSSIDstr(i).c_str(), sizeof(list[i].bssid) - 1);
    list[i].ssid[32] = '\0';
    list[i].bssid[17] = '\0';

    list[i].rssi      = WiFi.RSSI(i);
    list[i].authMode  = WiFi.encryptionType(i);
    list[i].channel   = WiFi.channel(i);
  }
  return len;
}

void printNetworks(){

  WifiNetworkList list;
  int len = scanWifiNetworks(list);

  for(int i = 0; i < len; i++){
    Serial.printf("(%d) %s\n", list[i].rssi, list[i].ssid);
  }

}

bool findNetwork(){
  static WifiNetworkList availb;
  static WifiSavedList   saved;
  
  int len1 = scanWifiNetworks(availb);
  int len2 = readSavedNetwork(saved);

  if(len1 <= 0 || len2 <= 0) return false;

  for(int i = 0; i < len1; i++){

    for(int j = 0; j < len2; j++){

      if(!saved[j].used) continue;

      if( strcmp(availb[i].ssid, saved[j].ssid) == 0){

        if( connectWifi(saved[j].ssid, saved[j].pass) == WIFI_OK){
          return true;
        }

      }
    }
  }

  return false;
}

int initWifi(){

  bool find = findNetwork();

  if(find){
    printWifiStatus();
    WIFIconnected = true;

    webServerInit();

    //initMqtt();

    return 1;
  }
  else{
    WIFIconnected = false;
    webServerAPInit();
    return 0;
  }

  // return 1;
}

void printWifiStatus(){
  Serial.print("Conectado à rede Wifi: [");
  Serial.print(WiFi.SSID());
  Serial.print("] {");
  Serial.print(WiFi.localIP());
  Serial.println("}");
}

