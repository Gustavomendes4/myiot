#ifndef NETWORK_WIFI_HPP_INCLUDED
#define NETWORK_WIFI_HPP_INCLUDED

#include <string>
#include <optional>

#include "IPAddress.hpp"

class Wifi{

    public:

    enum class Status{
        WIFI_OK,
    };

    enum class SecurityType{
        OPEN,       // Open network
        WEP,        // Wep network
        WPA2_PSK,   // WPA2 Personal
        WPA3_SAE,   // WPA3 Personal
        WPA2_EAP    // WPA2 Enterprise
    };

    class Network{

        std::string ssid;

        std::string bssid;

        std::string password;
    };

    class Credentials{

    };

};

#endif // NETWORK_WIFI_HPP_INCLUDED