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
};

#endif // TIME_DATA_H