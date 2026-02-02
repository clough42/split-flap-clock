#include "ConfigPersistence.h"

ConfigPersistence::ConfigPersistence() {}

void ConfigPersistence::initialize() {
    EEPROM.begin(EEPROM_SIZE);
}


int ConfigPersistence::getTimezoneOffset() {
    int offset = EEPROM.read(TIMEZONE_OFFSET_ADDR);
    if (offset >= 0 && offset <= 23) {
        return offset;
    }
    return 8;
}


void ConfigPersistence::setTimezoneOffset(int offset) {
    EEPROM.write(TIMEZONE_OFFSET_ADDR, offset);
    EEPROM.commit();
}

// 12/24 hour format persistence
bool ConfigPersistence::getIs24HourFormat() {
    uint8_t val = EEPROM.read(TIME_FORMAT_ADDR);
    return (val == 1);
}

void ConfigPersistence::setIs24HourFormat(bool is24Hour) {
    EEPROM.write(TIME_FORMAT_ADDR, is24Hour ? 1 : 0);
    EEPROM.commit();
}
