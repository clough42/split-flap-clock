#pragma once
#include <EEPROM.h>

class ConfigPersistence {
public:
    ConfigPersistence();
    void initialize();
    int getTimezoneOffset();
    void setTimezoneOffset(int offset);
private:
    static constexpr int TIMEZONE_OFFSET_ADDR = 0;
    static constexpr int EEPROM_SIZE = 1; // Size of EEPROM to allocate
};
