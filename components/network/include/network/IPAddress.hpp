#ifndef NETWORK_IPADRESS_INCLUDED
#define NETWORK_IPADRESS_INCLUDED

#include <string>
#include <stdint.h>

class IPAddress{

    // std::string address     = "";
    // std::string subnetMask  = "255.255.255.0";
    // std::string gateway     = "";
    // std::string primaryDns  = "";
    // std::string secondDns   = "";

    union{
        uint8_t bytes[4];
        uint32_t dword;
    }_address;

    uint8_t* raw_address(){ return _address.bytes; }

    public:
    
    // Constructors
    IPAddress();
    IPAddress(uint8_t first_byte, uint8_t second_byte, uint8_t third_byte, uint8_t fourth_byte);
    IPAddress(uint32_t rawAddr);
    IPAddress(const uint8_t* addr);

    bool fromString(const char *address);
    bool fromString(const std::string &strAddr);

    // Overload cast operator
    operator uint32_t() const;

    bool operator==(const IPAddress& addr) const;

    bool operator==(const uint8_t* addr) const;

    // Overload index operators
    uint8_t operator[](int index) const;
    uint8_t& operator[](int index);

    // Overload copy operators
    IPAddress& operator=(const uint8_t* address);
    IPAddress& operator=(uint32_t address);
};

#endif // NETWORK_IPADRESS_INCLUDED