#pragma once
#include <EEPROM.h>


class ConfigPersistence {
public:
    ConfigPersistence();
    void initialize();
    
    int getTimezoneOffset();
    void setTimezoneOffset(int offset);

    // 12/24 hour format support
    bool getIs24HourFormat();
    void setIs24HourFormat(bool is24Hour);

private:
    static constexpr int TIMEZONE_OFFSET_ADDR = 0;
    static constexpr int TIME_FORMAT_ADDR = 1;
    static constexpr int EEPROM_SIZE = 2; // Increased for time format
};
