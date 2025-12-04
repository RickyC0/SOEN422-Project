#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "esp_camera.h"
#include "SystemConfig.h"

// --- FREENOVE WROVER PINOUT ---
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22
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
    Serial.println("Camera Init Failed!");
    return;
  }
}

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWi-Fi Connected!");
}

void initFirebase() {
  config.api_key = FIREBASE_API_KEY; 
  
  // --- ANONYMOUS SETUP ---
  // This tells the library to use Anonymous / Guest sign-in
  config.signer.test_mode = true; 
  
  // Set the timeout longer for slow photo uploads
  config.timeout.serverResponse = 10 * 1000; 

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setup() {
  Serial.begin(115200);
  
  initCamera();
  initWiFi();
  initFirebase();
}

void loop() {
  if (Firebase.ready()) {
    Serial.println("Taking Picture...");
    
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera Capture Failed");
      return;
    }

    Serial.printf("Picture taken! Size: %d bytes. Uploading...\n", fb->len);

    // Upload using STORAGE_BUCKET_ID from SystemConfig.h
    if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID_WEB, fb->buf, fb->len, "plant_photos/plant1.jpg", "image/jpeg")) {
        Serial.println(">> Upload Success!");
    } else {
        Serial.printf(">> Upload Failed: %s\n", fbdo.errorReason().c_str());
    }

    esp_camera_fb_return(fb);
    
    // Wait 1 minute
    Serial.println("Sleeping for 1 minute...");
    delay(1 * 60 * 1000); 
  }
}