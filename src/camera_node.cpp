#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "esp_camera.h"
#include "SystemConfig.h"
#include "DebugMacros.h"

// Objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool wifiConnected = false;

void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 

  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // 1600x1200
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  if (esp_camera_init(&config) != ESP_OK) {
   LOG("Camera Init Failed!");
    return;
  }
}

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  LOG("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    LOG(".");
    delay(WIFI_DELAY_MS);
  }
 LOG("\nWi-Fi Connected!");
}

void initFirebase() {
  config.api_key = FIREBASE_API_KEY; 
  
  // --- ANONYMOUS SETUP ---
  // This tells the library to use Anonymous / Guest sign-in
  config.signer.test_mode = true; 
  
  // Set the timeout longer for slow photo uploads
  config.timeout.serverResponse = SERVER_PICTURE_RESPONSE_TIMEOUT_MS; 

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  
  initCamera();
  initWiFi();
  initFirebase();
}

void loop() {
    // Reconnect Wi-Fi if disconnected at anytime during runtime
    if (WiFi.status() != WL_CONNECTED) {
        initWiFi();
    }

    if (Firebase.ready()) {
       LOG("Taking Picture...");

        camera_fb_t * fb = esp_camera_fb_get();
        if (!fb) {
           LOG("Camera Capture Failed");
            return;
        }

        Serial.printf("Picture taken! Size: %d bytes. Uploading...\n", fb->len);

        // Upload using STORAGE_BUCKET_ID from SystemConfig.h
        if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID_WEB, fb->buf, fb->len, "plant_photos/plant1.jpg", "image/jpeg")) {
           LOG(">> Upload Success!");
        } else {
            Serial.printf(">> Upload Failed: %s\n", fbdo.errorReason().c_str());
        }

        esp_camera_fb_return(fb);

        // Wait 1 minute
       LOG("Sleeping for 1 minute...");
        delay(CAMERA_UPLOAD_INTERVAL_MS); 
    }
}