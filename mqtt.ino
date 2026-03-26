
#include "definition.h"

WiFiClient espClient;
PubSubClient MQTT(espClient);

//mosquitto_sub -h test.mosquitto.org -p 1883 -t g7f3b92e1c4a8d29f0b1 -v
//mosquitto_pub -h test.mosquitto.org -p 1883 -t g7f3b92e1c4a8d29f0b1 -m "teste"

const char* BROKER_MQTT = "test.mosquitto.org";
const int   BROKER_PORT = 1883;
const char* ID_MQTT     = "MYIOT";

const char* TOPIC_MQTT  = "g7f3b92e1c4a8d29f0b1";

const char* MQTT_USER = "usuario";
const char* MQTT_PASS = "senha";

const int   mqtt_tries = 10;

void printMqttInfo(const char* id = "none"){
  Serial.print("\tBroker/Port: ");
  Serial.print(BROKER_MQTT);

  Serial.print(":");
  Serial.println(BROKER_PORT);

  Serial.print("\tID: ");
  Serial.println(id);
}

bool mqtt(bool verb){
  int i = 0;
  String id = ID_MQTT + String(":") + String(random(0xFFFF), HEX);

  if(MQTT.connected())
    return true;

  if(verb)
    Serial.print("\nConectando ao broker");
  
  while(!MQTT.connected()){

    if(MQTT.connect(id.c_str())){
      if(verb){
        Serial.print(": Conectado!\n");
        printMqttInfo(id.c_str());
      }
      Serial.println();
      return true;
    }

    else{
      if(verb){
        Serial.print(".");
      }
    
      if(i++ == mqtt_tries){
        break;
      }
    }
  }

  if(verb){
    Serial.println("Falha em conectar-se ao Broker.\n");
  }
  return false;
}

void mqtt_callback(char* topic, byte* payload, unsigned int length){//função que recebe mensagens dos tópicos assinados
  /*
  String topico = topic;
  String mensagem;//obtem a string do payload recebido
  for(int i = 0; i < length; i++){
     char c = (char)payload[i];
     mensagem += c;
  }  
  if(topico.equals(TOPICO_SUBSCRIBE_PORTA_4)){//lê o topico "UERGS/Porta_4" 
    //toma ação dependendo da string recebida:
    if(mensagem.equals("On")){
      Serial.println("Porta 4 mandou on");
      Porta_4 = 1;
    }
    if(mensagem.equals("Off")){
      Serial.println("Porta 4 mandou off");
      Porta_4 = 0;
    }
  }
  */
}

void initMqtt(){
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
  mqtt(true);
}

void sendMqtt(){
  String json = createJson();
  MQTT.publish(TOPIC_MQTT, json.c_str(), true);
}

void mqttLoop(){
  MQTT.loop();
}

