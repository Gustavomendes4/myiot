

# Sistema, Logs e Diagnóstico
/* Controle total do system */
#include "esp_system.h"

/* Codigos de erro padrao */
#include "esp_err.h"

/* Parageracao de LOGS formatados no console */
#include "esp_log.h"

/* Todas as macros configuradas via idf.py menuconfig */
#include "sdkconfig.h"

/* Modelo do SoC, revisão do silício e contagem de núcleos */
#include "esp_chip_info.h"


# FreeRTOS
/* Base */
#include "freertos/FreeRTOS.h"

/* Gerenciamento de task*/
#include "freertos/task.h"

/* Gerenciamento de task*/
#include "freertos/queue.h"

/* Semaforos de mutexes */
#include "freertos/semphr.h"

/* Flags e sincronização por bits */
#include "freertos/event_groups.h"

/* Timers via software executados na daemon task do RTOS */
#include "freertos/timers.h"



# Periféricos e Drivers de Hardware
/* Pinos digitais, pull-up/pull-down e interrupções externas (ISR) */
#include "driver/gpio.h"

/* Comunicação serial UART / RS-485 */
#include "driver/uart.h"

/* Barramento I2C */
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"

/* Barramento SPI */
#include "driver/spi_master.h"
#include "driver/spi_slave.h"

/* Geração de PWM via hardware (LEDs, servo-motores, controle de potência) */
#include "driver/ledc.h"

/* Timers de hardware de propósito geral com interrupção de alta precisão */
#include "driver/gptimer.h"

/* Contador de pulsos por hardware (PCNT, ideal para encoders) */
#include "driver/pulse_cnt.h"

/* Controle remoto / geração de sinais digitais precisos (ex: fitas WS2812) */
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"

/* Conversor Analógico-Digital (ADC) */
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_continuous.h"


# Timers, Energia e Memória
/* Timer monotônico de alta resolução em microssegundos (esp_timer_get_time()) */
#include "esp_timer.h"

/* Gerenciamento de Deep Sleep, Light Sleep e fontes de wakeup */
#include "esp_sleep.h"

/* Inspeção e alocação segmentada de RAM interna, DMA e PSRAM (heap_caps_malloc()) */
#include "esp_heap_caps.h"

/* Inicialização e verificação de memória PSRAM externa */
#include "esp_psram.h"


# Armazenamento e Partições
/* Armazenamento não-volátil chave-valor na Flash */
#include "nvs_flash.h"
#include "nvs.h"

// Acesso direto a partições brutas da memória Flash */
#include "esp_partition.h"

// Sistemas de arquivos montáveis na Flash interna */
#include "esp_spiffs.h"
//#include "esp_littlefs.h"

// Montagem de cartões SD (via SPI ou SDMMC) */
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

// Atualização de firmware Over-The-Air (OTA) */
#include "esp_ota_ops.h"


# Rede, Wi-Fi e Protocolos
/* Barramento de eventos do sistema (esp_event_loop_create_default()) */
#include "esp_event.h"

/* Abstração da pilha TCP/IP (LwIP) e DHCP */
#include "esp_netif.h"

/* Configuração e operação do rádio Wi-Fi (STA, AP, Scan) */
#include "esp_wifi.h"

/* Interface Ethernet (MAC interno + PHY externo) */
#include "esp_eth.h"

/* Sockets BSD padrão para comunicação TCP/UDP bruta */
#include "lwip/sockets.h"
#include "lwip/netdb.h"

/* Cliente e servidor HTTP/HTTPS */
#include "esp_http_client.h"
#include "esp_http_server.h"

/* Cliente MQTT nativo (esp-mqtt) */
#include "mqtt_client.h"

/* Conexões seguras TLS/SSL e pacote de certificados raiz */
#include "esp_tls.h"
#include "esp_crt_bundle.h"

# Bluetooth (BLE / Clássico)
/* Controle do controlador Bluetooth */
#include "esp_bt.h"

/* Pilha leve NimBLE (padrão recomendada para BLE) */
#include "nimble/nimble_port.h"
#include "host/ble_hs.h"

/* Pilha Bluedroid (quando usado Bluetooth Clássico + BLE) */
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"


Flash and upload
// idf.py -p PORTA flash monitor
