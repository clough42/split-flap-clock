#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LEDController {
private:
    int ledPin_;
    bool ledState_;
    
public:
    LEDController(int pin);
    
    void setStatus(bool on);
    void toggle();
    void initialize();
};

#endif // LED_CONTROLLER_H