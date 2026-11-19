
// #include "esp_system.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/uart.h"
// #include "esp_wifi.h"
// #include "lwip/sockets.h"
// #include "esp_http_client.h"
// #include "esp_http_server.h"
// #include "mqtt_client.h"

// #include "esp_spiffs.h"
// #include "esp_littlefs.h"


#include "myiot.h"
#include "myiot/Gpio.hpp"

#include "network/IPAddress.hpp"

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>

// using namespace ;

void myiot_main(){

    Gpio gpio;

    IPAddress ip;

    std::string meu_ip = "192.168.10.20";

    ip.fromString(meu_ip);
    
    gpio.begin();

    for(int i = 0; i < 10; i++){
        gpio.led(true);
        vTaskDelay( pdMS_TO_TICKS(100) );
        gpio.led(false);
        vTaskDelay( pdMS_TO_TICKS(100) );
    }

    while(1){
        vTaskDelay( pdMS_TO_TICKS(100) );

        if( gpio.button() ){
            printf("Botao pressionado!\n");
        }

        gpio.led( gpio.button() );

    }
}
