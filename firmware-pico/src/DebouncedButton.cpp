#include "DebouncedButton.h"


DebouncedButton::DebouncedButton(uint8_t pin, unsigned long debounceMs, unsigned long longPressMs)
    : pin_(pin), debounceMs_(debounceMs), longPressMs_(longPressMs), lastDebounceTime_(0), lastStableState_(HIGH), lastRawState_(HIGH), buttonDownTime_(0), longPressDetected_(false), shortPressDetected_(false), waitingForRelease_(false) {
}


void DebouncedButton::initialize() {
    pinMode(pin_, INPUT_PULLUP);
    lastStableState_ = digitalRead(pin_);
    lastRawState_ = lastStableState_;
    lastDebounceTime_ = millis();
    buttonDownTime_ = 0;
    longPressDetected_ = false;
    shortPressDetected_ = false;
    waitingForRelease_ = false;
}


void DebouncedButton::update() {
    bool reading = digitalRead(pin_);
    unsigned long currentTime = millis();

    if (reading != lastRawState_) {
        lastDebounceTime_ = currentTime;
        lastRawState_ = reading;
    }

    if ((currentTime - lastDebounceTime_) >= debounceMs_) {
        if (lastStableState_ != lastRawState_) {
            lastStableState_ = lastRawState_;
            if (lastStableState_ == LOW) {
                // Button just pressed
                buttonDownTime_ = currentTime;
                longPressDetected_ = false;
                shortPressDetected_ = false;
                waitingForRelease_ = true;
            } else {
                // Button just released
                if (waitingForRelease_) {
                    if (!longPressDetected_) {
                        shortPressDetected_ = true;
                    }
                    waitingForRelease_ = false;
                }
            }
        }
    }

    // If button is held and not yet reported as long press
    if (waitingForRelease_ && !longPressDetected_ && lastStableState_ == LOW) {
        if ((currentTime - buttonDownTime_) >= longPressMs_) {
            longPressDetected_ = true;
            shortPressDetected_ = false; // Prevent short press
        }
    }
}

bool DebouncedButton::wasShortPressed() {
    if (shortPressDetected_) {
        shortPressDetected_ = false;
        return true;
    }
    return false;
}

bool DebouncedButton::wasLongPressed() {
    if (longPressDetected_) {
        longPressDetected_ = false;
        return true;
    }
    return false;
}
