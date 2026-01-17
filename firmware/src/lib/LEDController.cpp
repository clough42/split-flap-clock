#include "LEDController.h"

LEDController::LEDController(int pin, unsigned long interval)
    : ledPin_(pin), ledState_(false), previousMillis_(0), blinkInterval_(interval) {
}

void LEDController::initialize() {
    pinMode(ledPin_, OUTPUT);
}

void LEDController::blink() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis_ >= blinkInterval_) {
        previousMillis_ = currentMillis;
        ledState_ = !ledState_;
        digitalWrite(ledPin_, ledState_);
    }
}

void LEDController::setStatus(bool on) {
    ledState_ = on;
    digitalWrite(ledPin_, ledState_);
}