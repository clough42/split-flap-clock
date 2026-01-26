#ifndef DEBOUNCEDBUTTON_H
#define DEBOUNCEDBUTTON_H

#include <Arduino.h>

class DebouncedButton {
public:
    DebouncedButton(uint8_t pin, unsigned long debounceMs);
    
    void initialize();
    bool pressed();

private:
    uint8_t pin_;
    unsigned long debounceMs_;
    unsigned long lastPressTime_;
    bool lastState_;
};

#endif
