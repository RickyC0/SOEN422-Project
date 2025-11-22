#include "Plant.h"

Plant::Plant(int pumpPin, int moistPin, int lightPin, 
             int rPin, int gPin, int bPin,
             int dryVal, int wetVal,
             int moistThresh, int lightThresh,
             unsigned long pumpDur, unsigned long pumpCool,
             unsigned long moistInt, 
             unsigned long lightInt, unsigned long pauseInt)
    : _pump(pumpPin),
      _moisture(moistPin, dryVal, wetVal),
      _lightSensor(lightPin)
{
    // Save Pins
    _pinRed = rPin;
    _pinGreen = gPin;
    _pinBlue = bPin;

    // Save Configs
    _moistThreshold = moistThresh;
    _lightThreshold = lightThresh;
    _pumpDuration = pumpDur;
    _pumpCooldown = pumpCool;
    _moistCheckInterval = moistInt;
    _lightCheckInterval = lightInt;
    _pauseLightInterval = pauseInt;

    // Init States & Timers
    _waterState = W_IDLE;
    _lightingActive = false;
    _waterTimer = 0;
    _lightCheckTimer = 0;
    _moistureCheckTimer = 0;
    _isPeekingLight = false;
    _peekLightTimer = 0;
}

void Plant::begin() {
    _pump.begin();
    _moisture.begin();
    _lightSensor.begin();
    pinMode(_pinRed, OUTPUT);
    pinMode(_pinGreen, OUTPUT);
    pinMode(_pinBlue, OUTPUT);

    LOG("System Initialized.");
    LOG("Settings -> Moist Thresh: %d%% | Light Thresh: %d", _moistThreshold, _lightThreshold);
}

void Plant::update() {
    unsigned long now = millis();
    manageWatering(now);
    manageLighting(now);
}

void Plant::manageWatering(unsigned long now) {
    switch (_waterState) {
        case W_IDLE:
            if (now - _moistureCheckTimer > _moistCheckInterval) { 
                _moistureCheckTimer = now; 
                
                int currentMoist = _moisture.getPercentage();
                LOG("Checking Soil... Level: %d%% (Threshold: %d%%)", currentMoist, _moistThreshold);

                if (currentMoist < _moistThreshold) {
                    LOG("!!! THIRSTY DETECTED !!! Starting Pump for %lu ms.", _pumpDuration);
                    _pump.turnOn();
                    _waterTimer = now; 
                    _waterState = W_PUMPING;
                }
            }
            break;

        case W_PUMPING:
            if (now - _waterTimer > _pumpDuration) {
                LOG("Watering Complete. Pump OFF. Entering Cooldown.");
                _pump.turnOff();
                _waterTimer = now; 
                _waterState = W_COOLDOWN;
            }
            break;

        case W_COOLDOWN:
            if (now - _waterTimer > _pumpCooldown) {
                LOG("Cooldown finished. Resuming monitoring.");
                _waterState = W_IDLE;
            }
            break;
    }
}

void Plant::manageLighting(unsigned long now) {
    
    // --- PHASE 1: Handle the "Peeking" (The Pause) ---
    if (_isPeekingLight) {
        // Has the pause time passed?
        if (now - _peekLightTimer > _pauseLightInterval) {
            
            // Time is up! Read sensor
            int lightLevel = _lightSensor.getLightStatus();
            
            LOG("Peek Complete. Ambient Light Level: %d", lightLevel);
                
            if (lightLevel <= _lightThreshold) {
                LOG("It is Dark (Level %d <= %d). Lights ON.", lightLevel, _lightThreshold);
                analogWrite(_pinRed, 255);
                analogWrite(_pinGreen, 255);
                analogWrite(_pinBlue, 255);
                _lightingActive = true;
            } else {
                LOG("It is Bright (Level %d > %d). Lights OFF.", lightLevel, _lightThreshold);
                // Keep them off
                _lightingActive = false;
            }

            // Reset flags
            _isPeekingLight = false;
            _lightCheckTimer = now; // Start the long timer now
        }
        return; // STOP HERE.
    }


    // --- PHASE 2: Trigger the Check ---
    if (now - _lightCheckTimer > _lightCheckInterval) {
        
        LOG("Time to check light...");

        // Scenario A: Lights are ON. We must pause.
        if (_lightingActive) {
            LOG("Lights are ON. Turning OFF briefly to peek at ambient light.");
            analogWrite(_pinRed, 0);
            analogWrite(_pinGreen, 0);
            analogWrite(_pinBlue, 0);
            
            _isPeekingLight = true;
            _peekLightTimer = now; 
        } 
        
        // Scenario B: Lights are OFF. Instant check.
        else {
            int lightLevel = _lightSensor.getLightStatus();
            LOG("Lights are OFF. Current Light Level: %d", lightLevel);

            if (lightLevel <= _lightThreshold) {
                LOG("Too Dark. Turning Lights ON.");
                analogWrite(_pinRed, 255);
                analogWrite(_pinGreen, 255);
                analogWrite(_pinBlue, 255);
                _lightingActive = true;
            }
            else {
                _lightingActive = false;
            }
            
            _lightCheckTimer = now; 
        }
    }
}