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
