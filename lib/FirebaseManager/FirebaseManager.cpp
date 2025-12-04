#include "FirebaseManager.h"
#include "SystemConfig.h"
#include "DebugMacros.h"

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

FirebaseManager::FirebaseManager(String plantId) {
    _plantPath = "/plants/" + plantId;
    _isReady = false;
    
    // Initialize pointers to null (Safety)
    _fbdo = nullptr;
    _auth = nullptr;
    _config = nullptr;
}

void FirebaseManager::begin() {
    LOG("Initializing Firebase...");

    // --- ALLOCATE MEMORY NOW (SAFE) ---
    _fbdo = new FirebaseData();
    _auth = new FirebaseAuth();
    _config = new FirebaseConfig();

    // 1. Configure API Key and Database URL
    _config->api_key = FIREBASE_API_KEY;
    _config->database_url = FIREBASE_DB_URL; 

    // 2. Enable "Anonymous" Sign-in
    _config->signer.test_mode = true; 

    // 3. Start Connection
    Firebase.reconnectWiFi(true);
    
    // Pass the pointers
    Firebase.begin(_config, _auth);

    // 4. Wait for token
    int retries = 0;
    while ((_auth->token.uid == "") && (retries < 10)) {
        LOG("Connecting to Firebase...");
        delay(1000);
        retries++;
    }

    if (_auth->token.uid != "") {
        LOG("Firebase Connected! UID: %s", _auth->token.uid.c_str());
        _isReady = true;
    } else {
        LOG("Firebase Connection FAILED.");
    }
}

void FirebaseManager::sendData(int moisture, int lightStatus, bool pumpOn, bool lightOn) {
    if (!Firebase.ready()) return;

    FirebaseJson json;
    json.set("Moisture", moisture);
    json.set("Light", lightStatus);
    json.set("PumpState", pumpOn ? "ON" : "OFF");
    json.set("LightState", lightOn ? "ON" : "OFF");

    LOG("Pushing data to Cloud...");
    
    // Use the arrow -> for pointers
    if (Firebase.RTDB.updateNode(_fbdo, _plantPath.c_str(), &json)) {
        LOG("   >> Data Sent Successfully");
    } else {
        LOG("   >> Push Failed: %s", _fbdo->errorReason().c_str());
    }
}

void FirebaseManager::checkCommands(bool &pumpCmd, bool &lightCmd) {
    if (!Firebase.ready()) return;

    // Check Pump Command
    // Path: /plants/plant1/commands/pump
    String pumpPath = _plantPath + "/commands/pump";
    if (Firebase.RTDB.getBool(_fbdo, pumpPath)) {
        if (_fbdo->boolData() == true) {
            LOG("   >> COMMAND RECEIVED: Run Pump");
            pumpCmd = true;
            // IMMEDIATELY reset the command to false so it doesn't run forever!
            Firebase.RTDB.setBool(_fbdo, pumpPath, false); 
        }
    }

    // Check Light Toggle Command
    String lightPath = _plantPath + "/commands/lightToggle";
    if (Firebase.RTDB.getBool(_fbdo, lightPath)) {
        if (_fbdo->boolData() == true) {
            LOG("   >> COMMAND RECEIVED: Toggle Light");
            lightCmd = true;
            Firebase.RTDB.setBool(_fbdo, lightPath, false); // Reset trigger
        }
    }
}