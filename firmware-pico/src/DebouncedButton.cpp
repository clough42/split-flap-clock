#include "DebouncedButton.h"

DebouncedButton::DebouncedButton(uint8_t pin, unsigned long debounceMs)
    : pin_(pin), debounceMs_(debounceMs), lastPressTime_(0), lastState_(HIGH) {
}

void DebouncedButton::initialize() {
    pinMode(pin_, INPUT_PULLUP);
}

bool DebouncedButton::pressed() {
    bool currentState = digitalRead(pin_);
    unsigned long currentTime = millis();
    
    // Detect falling edge (HIGH -> LOW transition) with debouncing
    if (lastState_ == HIGH && currentState == LOW) {
        if (currentTime - lastPressTime_ >= debounceMs_) {
            lastPressTime_ = currentTime;
            lastState_ = currentState;
            return true;
        }
    }
    
    lastState_ = currentState;
    return false;
}
