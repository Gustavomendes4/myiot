idf_component_register(
    SRCS
        "myiot.cpp"
        "src/FileSystem.cpp"
        "src/Gpio.cpp"
        "src/MqttClient.cpp"

    INCLUDE_DIRS
        "include"

    REQUIRES
        network
        esp_driver_gpio
        esp_driver_uart
)