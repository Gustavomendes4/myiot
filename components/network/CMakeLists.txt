idf_component_register(
    SRCS
        "src/IPAdress.cpp"

    INCLUDE_DIRS
        "."
        "include"


    REQUIRES
        esp_wifi
        esp_http_client
        esp_http_server
)