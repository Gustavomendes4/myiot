#include "FS.h"
#include "LittleFS.h"

const char* fileName = "/networks.bin";

bool initFileSystem(){
  return LittleFS.begin(true);
}

void saveNetworkFS(String ssid, String pass){

  WifiSavedList list;

  readSavedNetwork(list);

  for(int i = 0; i < NETWORK_LIST_LEN; i++){

    if(!list[i].used || i == NETWORK_LIST_LEN - 1){
      list[i].used = true;

      strncpy(list[i].ssid, ssid.c_str(), sizeof(list[i].ssid) - 1);

      strncpy(list[i].pass, pass.c_str(), sizeof(list[i].pass) - 1);

      list[i].ssid[sizeof(list[i].ssid) - 1] = '\0';
      list[i].pass[sizeof(list[i].pass) - 1] = '\0';

      break;
    }
  }

  writeSavedNetwork(list);
}

int readSavedNetwork(WifiSavedList list){

  if(!LittleFS.exists(fileName)){
    createEmptyNetworksFile();
    return 0;
  }

  File file = LittleFS.open(fileName, "r");
  if(!file) return -1;

  int count = 0;

  for(int i = 0; i < NETWORK_LIST_LEN; i++){

    if(file.read((uint8_t*)&list[i], sizeof(WifiFileSaved)) == sizeof(WifiFileSaved)){

      list[i].ssid[32] = '\0';
      list[i].pass[40] = '\0';

      if(list[i].used){
        count++;
      }
    }
    else{
      list[i].used = false; // marca vazio se não conseguiu ler
    }
  }
  
  file.close();
  return count;
}

int writeSavedNetwork(WifiSavedList list){

  if(!LittleFS.exists(fileName)){
    createEmptyNetworksFile();
  }

  File file = LittleFS.open(fileName, "w");
  if(!file) return 1;

  for(int i = 0; i < NETWORK_LIST_LEN; i++){
    file.write((uint8_t*)&list[i], sizeof(WifiFileSaved)); // lê uma struct
  }

  file.close();
  return 2;
}

void createEmptyNetworksFile(){
  WifiSavedList list = {0};
  File file = LittleFS.open(fileName, "w");

  if(!file) return;

  for(int i = 0; i < NETWORK_LIST_LEN; i++){
    list[i].used = false;
    file.write((uint8_t*)&list[i], sizeof(WifiFileSaved));
  }

  file.close();
}

void listSavedNetworks(){
  WifiSavedList list;
  if(readSavedNetwork(list) < 0){
    Serial.println("Erro ao ler redes salvas.");
    return;
  }

  Serial.println("\nRedes Wi-Fi salvas:");
  for(int i = 0; i < NETWORK_LIST_LEN; i++){
    if(list[i].used){
      Serial.print("SSID: ");
      Serial.print(list[i].ssid);
      Serial.print(" | Senha: ");
      Serial.println(list[i].pass);
    }
  }
  Serial.println();
}

String readFile(const char* path){
  File file = LittleFS.open(path, "r");

  if (!file || file.isDirectory()) {
    return String("Error to open file");
  }

  String content;
  while (file.available()) {
    content += (char)file.read();
  }
  file.close();
  return content;
}

