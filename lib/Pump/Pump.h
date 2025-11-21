#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump {
  private:
    int _pin;

  public:
    // Constructor: just saves the pin number
    Pump(int pin);

    // Setup: Configures the pin as output and ensures pump is OFF
    void begin();

    // Actions
    void turnOn();
    void turnOff();
};

#endif