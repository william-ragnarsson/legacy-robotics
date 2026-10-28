#include "Arduino.h"
#include <esp_now.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
#include <SPI.h>

#define PI 3.14159265358979323846

TFT_eSPI tft = TFT_eSPI();

struct StructJoystick {
  int up, down, right, left;
};
StructJoystick js;

//*********************************
//MAIN VARIABLES!
//*********************************

const unsigned long frameRateInterval = 20;
unsigned long previousFrameTime = 0;

String activeScreen = "AchtungDieKurve";

//******************************************************************
//ACHTUNG DIE KURVE!
//******************************************************************

struct StructKurve{
  float angle;

  float x, y;
  int intX, intY, prevX, prevY;

  float moveSpeed = 0.25;
  int turnSpeed = 10;
  int travelDist;

  uint32_t color = TFT_WHITE;
};

StructKurve resetKurve;
StructKurve kurve1;

bool matrix[128][160] = {0};

void AchtungDieKurve(){
  UpdateKurveAngle(&kurve1);
  UpdateKurvePos(&kurve1);

  kurve1.intX = int(kurve1.x);
  kurve1.intY = int(kurve1.y);

  CheckKurveCollision(&kurve1);

  Extra(&kurve1);
}

void UpdateKurveAngle(StructKurve *S){
  S->angle += (js.left - js.right) * S->turnSpeed;
}

void UpdateKurvePos(StructKurve *S){
  S->x += cos(S->angle / 360 * PI);
  S->y -= sin(S->angle / 360 * PI);
}

void CheckKurveCollision(StructKurve *S){
  //Check for general collision;
  if((S->intX != S->prevX or S->intY != S->prevY) and matrix[S->intY][S->intX]){
    AchtungGameOver();
  }
  //Check for out of bounds
  else if(S->intX < 0 or S->intY < 0 or S->intX > 159 or S->intY > 127){
    AchtungGameOver();
  }
  //Check for upward diagonal collision
  else if(S->prevY - 1 == S->intY){
    if(S->prevX + 1 == S->intX      and     matrix[S->intY + 1][S->intX] and matrix[S->intY][S->intX - 1]){
      AchtungGameOver();
    }
    else if(S->prevX - 1 == S->intX     and     matrix[S->intY + 1][S->intX] and matrix[S->intY][S->intX + 1]){
      AchtungGameOver();
    }
  }
  //Check for downard diagonal collision
  else if(S->prevY + 1 == S->intY){
    if(S->prevX + 1 == S->intX      and     matrix[S->intY - 1][S->intX] and matrix[S->intY][S->intX - 1]){
      AchtungGameOver();
    }
    else if(S->prevX - 1 == S->intX     and     matrix[S->intY - 1][S->intX] and matrix[S->intY][S->intX + 1]){
      AchtungGameOver();
    }
  }
}

void Extra(StructKurve *S){
  S->travelDist++;
  if(S->travelDist < 250){
    //Update matrix values
    matrix[S->intY][S->intX] = true;
    //Draw kurve
    tft.drawPixel(S->x, S->y, S->color);
  }
  else if(S->travelDist >= 270) S->travelDist = 0;

  S->prevX = S->intX;
  S->prevY = S->intY;
}

void AchtungGameOver(){
  tft.fillScreen(TFT_BLACK);
  kurve1 = resetKurve;
  memset(matrix, 0, sizeof(matrix));
}

//******************************************************************
//MAIN LOOP
//******************************************************************

void setup(){
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(OnDataRecv);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void loop(){
  int currentTime = millis();

  if(currentTime - previousFrameTime >= frameRateInterval){
    previousFrameTime = currentTime;

    if(activeScreen == "AchtungDieKurve") AchtungDieKurve();
  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&js, incomingData, sizeof(js));
}