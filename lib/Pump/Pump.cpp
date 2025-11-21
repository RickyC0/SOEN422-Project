#include "Pump.h"

Pump::Pump(int pin) {
  _pin = pin;
}

void Pump::begin() {
  pinMode(_pin, OUTPUT);
  
  // SAFETY: Immediately turn off the pump so it doesn't run on startup
  digitalWrite(_pin, LOW); 
}

void Pump::turnOn() {
  digitalWrite(_pin, HIGH); // Assuming Relay Active HIGH (High = On)
}

void Pump::turnOff() {
  digitalWrite(_pin, LOW);  // Assuming Relay Active HIGH (Low = Off)
}