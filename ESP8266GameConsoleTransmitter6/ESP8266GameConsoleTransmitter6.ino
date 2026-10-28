//ESP NOW import
#include <ESP8266WiFi.h>
#include <espnow.h>
//ESP NOW nintendo
#include <Wire.h>
#include <NintendoExtensionCtrl.h>

#define SCL1 5
#define SDA1 4

#define SCL2 0
#define SDA2 2

uint8_t broadcastAddress[] = { 0xB8, 0xD6, 0x1A, 0x44, 0x3B, 0x4C };

void ESPInit(){
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

ExtensionPort port;

ClassicController::Shared classic(port);
Nunchuk::Shared nchuk(port);

Nunchuk nunChuk;

struct struct_message {
  bool c1, Z1, c2, Z2;
  int joyX1, joyY1, joyX2, joyY2;
};
struct_message structNunChuk;

void setup() {
  Serial.begin(115200);

  ESPInit();
}

void loop() {
  Wire.begin(SDA1, SCL1);

  esp_now_send(broadcastAddress, (uint8_t *)&structNunChuk, sizeof(structNunChuk));
}

void PrintValues(){
  Serial.print(structNunChuk.joyX1); Serial.print(" ");
  Serial.print(structNunChuk.joyY1); Serial.print(" ");
  Serial.print(structNunChuk.Z1); Serial.print(" ");
  Serial.print(structNunChuk.c1); Serial.print(" ");

  Serial.print("     ");

  Serial.print(structNunChuk.joyX2); Serial.print(" ");
  Serial.print(structNunChuk.joyY2); Serial.print(" ");
  Serial.print(structNunChuk.Z2); Serial.print(" ");
  Serial.println(structNunChuk.c2);
}