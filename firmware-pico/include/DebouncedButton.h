#ifndef DEBOUNCEDBUTTON_H
#define DEBOUNCEDBUTTON_H

#include <Arduino.h>


class DebouncedButton {
public:
    DebouncedButton(uint8_t pin, unsigned long debounceMs, unsigned long longPressMs);
    void initialize();
    void update();
    bool wasShortPressed();
    bool wasLongPressed();

private:
    uint8_t pin_;
    unsigned long debounceMs_;
    unsigned long longPressMs_;
    unsigned long lastDebounceTime_;
    bool lastStableState_;
    bool lastRawState_;
    unsigned long buttonDownTime_;
    bool longPressDetected_;
    bool shortPressDetected_;
    bool waitingForRelease_;
};

#endif
