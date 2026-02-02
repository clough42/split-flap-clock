#ifndef TIME_DATA_H
#define TIME_DATA_H

// Time data structure for display and GPS communication
struct TimeData {
    int utcHours;
    int utcMinutes;
    int utcSeconds;
    int localHours;
    int localMinutes;
    int localSeconds;
    int satelliteCount;  // Number of satellites used in fix
    bool validTime;
    const char* signalStrength;  // Signal quality assessment
    bool is24HourFormat; // true = 24h, false = 12h
    bool isPm; // true = PM, false = AM (only valid if !is24HourFormat)
};

#endif // TIME_DATA_H