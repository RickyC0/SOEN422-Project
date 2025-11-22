#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <Arduino.h>

class LightSensor {
  private:
    int _pin;

  public:
    // Constructor
    LightSensor(int pin);

    // Setup: Configures ADC and pin mode
    void begin();

    // Returns the raw number (0 - 4095)
    int readRaw();

    
    int getLightStatus();

    String printLightStatus();

    int readAverage();
};

#endif