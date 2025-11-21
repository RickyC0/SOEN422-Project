#ifndef MOISTURE_SENSOR_H
#define MOISTURE_SENSOR_H

#include <Arduino.h>

class MoistureSensor {
  private:
    int _pin;
    int _dryVal;
    int _wetVal;

  public:
    // Constructor
    MoistureSensor(int pin, int dryVal, int wetVal);

    // Methods
    void begin();
    int getPercentage();
};

#endif