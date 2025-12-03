#ifndef PLANT_H
#define PLANT_H

#include <Arduino.h>
#include "DebugMacros.h"
#include "SystemConfig.h"

#include "Pump.h"
#include "MoistureSensor.h"
#include "LightSensor.h"

class Plant {
  private:
    // --- COMPONENTS ---
    Pump _pump;
    MoistureSensor _moisture;
    LightSensor _lightSensor;

    // --- PINS ---
    int _pinRed, _pinGreen, _pinBlue;

    // --- STATE VARIABLES ---
    enum WateringState { W_IDLE, W_PUMPING, W_COOLDOWN };
    WateringState _waterState;
    bool _lightingActive;

    // --- TIMERS (For tracking time) ---
    unsigned long _waterTimer; // Tracks pump duration/cooldown
    unsigned long _lightCheckTimer; // Tracks light checking interval
    unsigned long _moistureCheckTimer; // Tracks moisture checking interval

    // --- SETTINGS (Now loaded from Config) ---
    int _moistThreshold;
    int _lightThreshold;
    unsigned long _pumpDuration;
    unsigned long _pumpCooldown;
    unsigned long _moistCheckInterval;
    unsigned long _lightCheckInterval;
    bool _isPeekingLight; 
    unsigned long _peekLightTimer; 
    unsigned long _pauseLightInterval;

  public:
    // Giant Constructor to accept all Configs
    Plant(int pumpPin, int moistPin, int lightPin, 
          int rPin, int gPin, int bPin,
          int dryVal, int wetVal,
          int moistThresh, int lightThresh,
          unsigned long pumpDur, unsigned long pumpCool,
          unsigned long moistInt, 
          unsigned long lightInt, unsigned long pauseInt);

    void begin();
    void update();
    void manageWatering(unsigned long now);
    void manageLighting(unsigned long now);

    // --- GETTERS FOR WEBSITE ---
    // These allow the website to peek at private variables
    int getMoisturePercent() { return _moisture.getPercentage(); }
    int getLightStatus() { return _lightSensor.getLightStatus(); }
    bool isPumpRunning() { return (_waterState == W_PUMPING); }
    bool isLightOn() { return _lightingActive; }
    String getPlantName() { return PLANT_NAME; }
};

#endif