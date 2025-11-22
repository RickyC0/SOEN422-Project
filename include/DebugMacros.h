#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include <Arduino.h>
#include "SystemConfig.h" // To get PLANT_NAME

// --- MASTER SWITCH ---
// Comment this line out to silence the entire system for production
#define ENABLE_DEBUG 

#ifdef ENABLE_DEBUG
    // This macro automatically adds the plant name and a new line
    // Usage: LOG("Value is %d", myValue);
    #define LOG(fmt, ...) Serial.printf("[ " PLANT_NAME " ] " fmt "\n", ##__VA_ARGS__)
#else
    // If debug is disabled, this replaces the code with nothing
    #define LOG(...) 
#endif

#endif