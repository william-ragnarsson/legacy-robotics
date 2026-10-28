#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct structController {
  bool connected;
  int dir;
} structController;

structController controller;

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len);

void setup() {
  Serial.begin(115200);
  InitESPNOW();
}

void loop() {
  Serial.println(controller.dir);
}

void InitESPNOW() {
  WiFi.mode(WIFI_STA); // Set WiFi to station mode
  if (esp_now_init() != ESP_OK) { // Initialize ESP-NOW
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv); // Register the receive callback
}

// Callback when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&controller, incomingData, sizeof(controller));
}