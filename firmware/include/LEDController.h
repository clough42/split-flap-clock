#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LEDController {
private:
    int ledPin_;
    bool ledState_;
    unsigned long previousMillis_;
    unsigned long blinkInterval_;
    
public:
    LEDController(int pin = 13, unsigned long interval = 167);
    
    void blink();
    void setStatus(bool on);
    void initialize();
};

#endif // LED_CONTROLLER_H