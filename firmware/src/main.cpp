#include <Arduino.h>
#include <AccelStepper.h>

// Timezone configuration
const int TIMEZONE_OFFSET_HOURS = -7; // Mountain Time (MST = UTC-7, MDT = UTC-6)

// Stepper motor configuration
const int STEPS_PER_POSITION = 160; // 200 * 8 microsteps / 10 positions
const int ENABLE_PIN = 8; // Active low

// Define the 4 stepper motors (step pin, direction pin)
AccelStepper hoursTens(AccelStepper::DRIVER, 0, 1);     // Hours tens digit
AccelStepper hoursOnes(AccelStepper::DRIVER, 2, 3);     // Hours ones digit  
AccelStepper minutesTens(AccelStepper::DRIVER, 4, 5);   // Minutes tens digit
AccelStepper minutesOnes(AccelStepper::DRIVER, 6, 7);   // Minutes ones digit

// Forward-only motor class for AccelStepper
class ForwardOnlyMotor {
private:
  AccelStepper* motor;
  int currentPosition;  // 0-9
  int stepsPerPosition;
  String motorName;
  
public:
  ForwardOnlyMotor(AccelStepper* stepperMotor, int stepsPerPos, String name) 
    : motor(stepperMotor), currentPosition(0), stepsPerPosition(stepsPerPos), motorName(name) {
    motor->setMaxSpeed(1000);        // Steps per second
    motor->setAcceleration(2000);    // Steps per second²
  }
  
  void moveToDigit(int targetDigit) {

    if (targetDigit < 0 || targetDigit > 9) return;
    
    // Calculate forward-only distance
    int forwardSteps;
    if (targetDigit >= currentPosition) {
      // Normal forward movement
      forwardSteps = (targetDigit - currentPosition) * stepsPerPosition;
    } else {
      // Wrap around (go past 9 back to target)
      forwardSteps = ((10 - currentPosition) + targetDigit) * stepsPerPosition;
    }
    
    if (forwardSteps > 0) {
      motor->move(forwardSteps);
    }
    currentPosition = targetDigit;
  }
  
  bool isMoving() { 
    return motor->isRunning(); 
  }
  
  void setCurrentPosition(int position) {
    currentPosition = position;
  }
};

// Create motor controllers
ForwardOnlyMotor motorHoursTens(&hoursTens, STEPS_PER_POSITION, "Hours-Tens");
ForwardOnlyMotor motorHoursOnes(&hoursOnes, STEPS_PER_POSITION, "Hours-Ones");
ForwardOnlyMotor motorMinutesTens(&minutesTens, STEPS_PER_POSITION, "Minutes-Tens");
ForwardOnlyMotor motorMinutesOnes(&minutesOnes, STEPS_PER_POSITION, "Minutes-Ones");

String gpsBuffer = ""; // Buffer to store incoming GPS data

// LED blink variables
const int ledPin = 13; // Teensy 4.0 onboard LED
bool ledState = false;
unsigned long previousMillis = 0;
const unsigned long blinkInterval = 167; // ~3 blinks per second (1000ms/6 = 167ms per half cycle)

// Function to parse and display GPS time from NMEA sentence
void parseGPSTime(String nmea) {
  // Look for GPGGA sentences which contain time data
  if (nmea.startsWith("$GPGGA") || nmea.startsWith("$GNGGA")) {
    // Split the sentence by commas
    int firstComma = nmea.indexOf(',');
    int secondComma = nmea.indexOf(',', firstComma + 1);
    
    if (firstComma != -1 && secondComma != -1) {
      // Extract time field (between first and second comma)
      String timeStr = nmea.substring(firstComma + 1, secondComma);
      
      if (timeStr.length() >= 6) {
        // Parse HHMMSS.SS format
        int utcHours = timeStr.substring(0, 2).toInt();
        int utcMinutes = timeStr.substring(2, 4).toInt();
        int utcSeconds = timeStr.substring(4, 6).toInt();
        
        // Calculate local time
        int localHours = utcHours + TIMEZONE_OFFSET_HOURS;
        
        // Handle day rollover
        if (localHours < 0) {
          localHours += 24;
        } else if (localHours >= 24) {
          localHours -= 24;
        }
        
        // Display UTC time
        Serial.print("GPS Time (UTC): ");
        if (utcHours < 10) Serial.print("0");
        Serial.print(utcHours);
        Serial.print(":");
        if (utcMinutes < 10) Serial.print("0");
        Serial.print(utcMinutes);
        Serial.print(":");
        if (utcSeconds < 10) Serial.print("0");
        Serial.print(utcSeconds);
        
        // Add fractional seconds if available
        if (timeStr.length() > 6) {
          Serial.print(timeStr.substring(6));
        }
        
        // Display local time
        Serial.print(" | Local Time: ");
        if (localHours < 10) Serial.print("0");
        Serial.print(localHours);
        Serial.print(":");
        if (utcMinutes < 10) Serial.print("0");
        Serial.print(utcMinutes);
        Serial.print(":");
        if (utcSeconds < 10) Serial.print("0");
        Serial.print(utcSeconds);
        
        // Add fractional seconds if available
        if (timeStr.length() > 6) {
          Serial.print(timeStr.substring(6));
        }
        Serial.println();
        
        // Toggle LED to indicate successful GPS time parsing
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
        
        // Update stepper motors to display time
        motorHoursTens.moveToDigit(localHours / 10);
        motorHoursOnes.moveToDigit(localHours % 10);
        motorMinutesTens.moveToDigit(utcMinutes / 10);
        motorMinutesOnes.moveToDigit(utcMinutes % 10);
      }
    }
  }
}

// Background GPS processing - called automatically when Serial3 receives data
void serialEvent3() {
  while (Serial3.available()) {
    char gpsChar = Serial3.read();
    
    // Add character to buffer
    gpsBuffer += gpsChar;
    
    // Check if we have a complete line (NMEA sentences end with \r\n)
    if (gpsChar == '\n') {
      // Parse and display GPS time
      parseGPSTime(gpsBuffer);
      
      // Clear buffer for next line
      gpsBuffer = "";
    }
  }
}

void setup() {
  // Initialize LED pin
  pinMode(ledPin, OUTPUT);
  
  // Initialize stepper motor enable pin (active low)
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);  // Enable all motors
  
  // Initialize serial communication for console output
  Serial.begin(115200);
  Serial.println("GPS Split-Flap Clock Started");
  
  // Initialize UART3 for GPS communication (9600, N, 8, 1)
  Serial3.begin(9600);
  Serial.println("UART3 initialized for GPS at 9600 baud");
}

void loop() {
  // Run stepper motors (AccelStepper requires regular run() calls)
  hoursTens.run();
  hoursOnes.run();
  minutesTens.run();
  minutesOnes.run();
  
  // GPS processing happens automatically in serialEvent3()
}