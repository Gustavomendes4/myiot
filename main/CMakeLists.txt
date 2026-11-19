idf_component_register(
    SRCS
    "main.c"
    # "myiot.cpp"
    # "MyIoT/gpio.c"
    
    INCLUDE_DIRS
    "."

    REQUIRES
        myiot
        # esp_driver_gpio
        # esp_driver_uart
            # mqtt
        # esp_wifi
        # esp_http_client
        # esp_http_server
)
