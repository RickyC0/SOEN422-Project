#ifndef FIREBASE_MANAGER_H
#define FIREBASE_MANAGER_H

#include <Arduino.h>
#include <Firebase_ESP_Client.h>


class FirebaseManager {
  private:
    FirebaseData* _fbdo;
    FirebaseAuth* _auth;
    FirebaseConfig* _config;
    
    String _plantPath;
    bool _isReady;

  public:
    FirebaseManager(String plantId);
    void begin();
    void sendData(int moisture, int lightStatus, bool pumpOn, bool lightOn);
};

#endif