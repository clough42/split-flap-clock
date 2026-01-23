#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// ============================================================================
// COMMUNICATION SETTINGS
// ============================================================================
#define SERIAL_BAUD_RATE        115200  // Console serial port baud rate
#define GPS_BAUD_RATE           9600    // GPS serial port baud rate

// ============================================================================
// APPLICATION CONFIGURATION
// ============================================================================
#define TIMEZONE_OFFSET_HOURS   -7      // Mountain Time (MST = UTC-7, MDT = UTC-6)

// ============================================================================
// RP2040 HARDWARE CONFIGURATION
// ============================================================================
#define LED_PIN                 LED_BUILTIN  // Use Pico built-in LED (GPIO 25)

#endif // CONFIGURATION_H