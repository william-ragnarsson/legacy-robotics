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

Nunchuk nunChuk;

struct struct_message {
  bool connected1, connected2;
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
  if (!nunChuk.connect()) {
    // Serial.println("#1 NOT CONNECTED!");

    structNunChuk.joyX1 = 50;
    structNunChuk.joyY1 = 50;
    structNunChuk.Z1 = 0;
    structNunChuk.c1 = 0;

    structNunChuk.connected1 = false;
  }
  else{
    structNunChuk.connected1 = true;

    nunChuk.update();

    structNunChuk.joyX1 = map(nunChuk.joyX(), 35, 225, 0, 100);
    if(structNunChuk.joyX1 < 0) structNunChuk.joyX1 = 0;
    else if(structNunChuk.joyX1 > 100) structNunChuk.joyX1 = 100;

    structNunChuk.joyY1 = map(nunChuk.joyY(), 30, 220, 0, 100);
    if(structNunChuk.joyY1 < 0) structNunChuk.joyY1 = 0;
    else if(structNunChuk.joyY1 > 100) structNunChuk.joyY1 = 100;

    structNunChuk.Z1 = nunChuk.buttonZ();
    structNunChuk.c1 = nunChuk.buttonC();
  }

  Wire.begin(SDA2, SCL2);
  if (!nunChuk.connect()) {
    // Serial.println("#2 NOT CONNECTED!");

    structNunChuk.joyX2 = 50;
    structNunChuk.joyY2 = 50;
    structNunChuk.Z2 = 0;
    structNunChuk.c2 = 0;

    structNunChuk.connected2 = false;
  }
  else{
    structNunChuk.connected2 = true;

    nunChuk.update();

    structNunChuk.joyX2 = map(nunChuk.joyX(), 35, 225, 0, 100);
    if(structNunChuk.joyX2 < 0) structNunChuk.joyX2 = 0;
    else if(structNunChuk.joyX2 > 100) structNunChuk.joyX2 = 100;

    structNunChuk.joyY2 = map(nunChuk.joyY(), 30, 220, 0, 100);
    if(structNunChuk.joyY2 < 0) structNunChuk.joyY2 = 0;
    else if(structNunChuk.joyY2 > 100) structNunChuk.joyY2 = 100;

    structNunChuk.Z2 = nunChuk.buttonZ();
    structNunChuk.c2 = nunChuk.buttonC();
  }

  PrintValues();

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