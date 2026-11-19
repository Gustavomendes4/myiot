
/* Extern imports */
#include <cstring>

/* Intern imports */
#include "network/IPAddress.hpp"
    
// Constructors
IPAddress::IPAddress(){
    _address.dword = 0;
}

IPAddress::IPAddress(uint8_t first_byte, uint8_t second_byte, uint8_t third_byte, uint8_t fourth_byte){
    _address.bytes[0] = first_byte;
    _address.bytes[1] = second_byte;
    _address.bytes[2] = third_byte;
    _address.bytes[3] = fourth_byte;
}

IPAddress::IPAddress(uint32_t rawAddr){
    _address.dword = rawAddr;
}

IPAddress::IPAddress(const uint8_t* addr){
    memcpy(_address.bytes, addr, sizeof(_address.bytes));
}

bool IPAddress::fromString(const char *address){
    uint16_t acc = 0; // Accumulator
    uint8_t dots = 0;

    while (*address)
    {
        char c = *address++;
        if (c >= '0' && c <= '9')
        {
            acc = acc * 10 + (c - '0');
            if (acc > 255) {
                // Value out of [0..255] range
                return false;
            }
        }
        else if (c == '.')
        {
            if (dots == 3) {
                // Too much dots (there must be 3 dots)
                return false;
            }
            _address.bytes[dots++] = acc;
            acc = 0;
        }
        else
        {
            // Invalid char
            return false;
        }
    }

    if (dots != 3) {
        // Too few dots (there must be 3 dots)
        return false;
    }
    _address.bytes[3] = acc;
    return true;
}

bool IPAddress::fromString(const std::string &strAddr){
    return fromString( strAddr.c_str() );
}

// Overload cast operator
IPAddress::operator uint32_t() const{
    return _address.dword;
}

bool IPAddress::operator==(const IPAddress& addr) const {
    return _address.dword == addr._address.dword;
}

bool IPAddress::operator==(const uint8_t* addr) const{
    return memcmp(addr, _address.bytes, sizeof(_address.bytes)) == 0;
};

// Overload index operators
uint8_t IPAddress::operator[](int index) const {
    return _address.bytes[index];
}

uint8_t& IPAddress::operator[](int index){
    return _address.bytes[index];
}

// Overload copy operators
IPAddress& IPAddress::operator=(const uint8_t* address){
    memcpy(_address.bytes, address, sizeof(_address.bytes));
    return *this;
}

IPAddress& IPAddress::operator=(uint32_t address){
    _address.dword = address;
    return *this;
}
