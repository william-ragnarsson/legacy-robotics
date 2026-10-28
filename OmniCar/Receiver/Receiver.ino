// Includes and definitions
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct structController {
  bool connected;
  int dir;
};
structController controller;

void setup() {
  Serial.begin(115200);
  InitESPNOW();
}

void loop() {
  Serial.println(controller.dir);
}

void InitESPNOW() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&controller, incomingData, sizeof(controller));
}