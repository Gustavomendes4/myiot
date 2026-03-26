/*
  help

  fs  -> Filesystem commands

  cls -> clear terminal

  wifi  ->  wifi commands
    -> test
    -> status
    -> list
    -> disconnect
    -> connect {ssid} {password}
    -> reset

  mqtt  -> mqtt commands

  sys   -> system
*/

#include "prompt.h"

void promptLoop(){

  String cmd;
  int argc;
  String argv[MAX_ARGS];

  cmd = getCommand();

  if(cmd == "") return;

  argc = parseCommand(cmd, argv);


  Serial.print(">> " + cmd);

  commandDispatch(argc, argv);

  Serial.print("\n\n>> ");
}

int parseCommand(String input, String* vec){

  int argc = 0;


  while(input.length() > 0 && argc < MAX_ARGS){
    input.trim();

    int index = input.indexOf(' ');

    if(index == -1){
      vec[argc++] = input;
      break;
    }

    vec[argc++] = input.substring(0, index);

    input = input.substring(index + 1);
  }

  return argc;
}

CommandToken commandToken(String input){

  for(int i = 0; i < (int)LAST_CMD; i++){

    if( input == commands[i])
      return (CommandToken)i;
  }

  return INVALID_CMD;
}

String getCommand(){

  if( !Serial.available() )
    return String("");
  
  return Serial.readStringUntil('\n');
}

void commnadDebug(int argc, String argv[]){

  for(int i = 0; i < argc; i++){
    Serial.println(String(i) + ": " + argv[i]);
  }
}

void commandDispatch(int argc, String argv[]){
  CommandToken token = commandToken(argv[0]);

  switch( token ){
    case JSON_CMD:
      Serial.print(String("\n") + createJson());
      break;

    case WIFI_CMD:  
      listSavedNetworks();
      break;

    case MQTT_CMD:
      sendMqtt();
      break;

    case SYS_CMD:
      ESP.restart();
      break;

    case CLS_CMD:
      Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
      break;

    case GPIO_CMD:
      output[0] = !output[0];
      break;

    default:
      Serial.print("\n'" + argv[0] + "' não é um comando reconhecido." );
  }
}




