#include "LEDController.h"

LEDController::LEDController(int pin)
    : ledPin_(pin), ledState_(false) {
}

void LEDController::initialize() {
    pinMode(ledPin_, OUTPUT);
}

void LEDController::setStatus(bool on) {
    ledState_ = on;
    digitalWrite(ledPin_, ledState_);
}

void LEDController::toggle() {
    ledState_ = !ledState_;
    digitalWrite(ledPin_, ledState_);
}