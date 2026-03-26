#ifndef PROMPT_H_INCLUDED
#define PROMPT_H_INCLUDED


#define MAX_ARGS 10

typedef enum CommandToken{
  INVALID_CMD = 0,
  CLS_CMD,
  HELP_CMD,
  FS_CMD,
  WIFI_CMD,
  MQTT_CMD,
  SYS_CMD,
  GPIO_CMD,

  JSON_CMD,

  LAST_CMD
}CommandToken;

const String commands[] = {
  "",
  "cls",
  "help",
  "fs",
  "wifi",
  "mqtt",
  "sys",
  "gpio",

  "json",

  ""
};





#endif