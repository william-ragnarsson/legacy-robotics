// Includes and definitions
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

#include "Tank.h"
#include "Car.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite tank = TFT_eSprite(&tft);

// **************************************************************
// CONTROLLER INPUT
// **************************************************************

typedef struct structNunChuk {
  bool connected1, connected2;
  bool c1, Z1, c2, Z2;
  int joyX1, joyY1, joyX2, joyY2;
};
structNunChuk n;

// **************************************************************
// TANKIE GAME
// **************************************************************

float x = 0;
float y = 0;
float angle = 0;

unsigned long snakeFrameRate = 10;
unsigned long prevSnakeFrameRate = 0;

void setup() {
  Serial.begin(115200);
  InitESPNOW();

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setPivot(85, 85);

  tank.createSprite(16, 16);
  tank.fillSprite(TFT_RED);
  tank.pushSprite(50, 50);
  tank.pushRotated(30);
}

void loop() {
  tank.fillSprite(TFT_BLACK);
  tank.pushSprite()

  x += float(n.joyX1) / 40;
  y -= float(n.joyY1) / 40;

  // int currentTime = millis(); 

  // if(currentTime - prevSnakeFrameRate >= snakeFrameRate){
  //   prevSnakeFrameRate = currentTime;

  //   tft.fillRect(x, y, 16, 16, TFT_BLACK);

  //   x += float(n.joyX1) / 40;
  //   y -= float(n.joyY1) / 40;

  //   tft.fillRect(x, y, 16, 16, TFT_RED);
  // }

  // Serial.println(x);
}

void InitESPNOW(){
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&n, incomingData, sizeof(n));
}