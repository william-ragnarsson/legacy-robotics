// Includes and definitions
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

// Assets
// #include "Tank19x19.h"
// #include "TankieBackground.h"

#include "AceRight.h"
#include "AceLeft.h"
#include "AceUp.h"
#include "AceDown.h"

#include "AceRightUp.h"
#include "AceLeftUp.h"
#include "AceRightDown.h"
#include "AceLeftDown.h"

#include "Level1.h"

#define ANGLE_CONSTANT 0.70710689672

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite backgroundSprite = TFT_eSprite(&tft);
TFT_eSprite tankSprite = TFT_eSprite(&tft);

// **************************************************************
// CONTROLLER INPUT
// **************************************************************

typedef struct structNunChuk {
  bool connected1, connected2;
  bool c1, Z1, c2, Z2;
  int joyX1, joyY1, joyX2, joyY2;
};
structNunChuk n;

bool nRight = false;
bool nLeft = false;
bool nRightClicked = false;
bool nLeftClicked = false;

bool nUp = false;
bool nDown = false;
bool nUpClicked = false;
bool nDownClicked = false;

bool nZ = false;
bool nc = false;
bool nZClicked = false;
bool ncClicked = false;

void ControllerClicked(){
  // Detection for right and left "click"
  if(n.joyX1 > 30 and nRight == false){
    nRight = true;
    nRightClicked = true;
  } else if(n.joyX1 < -30 and nLeft == false){
    nLeft = true;
    nLeftClicked = true;
  }

  if(n.joyY1 > 30 and nUp == false){
    nUp = true;
    nUpClicked = true;
  } else if(n.joyY1 < -30 and nDown == false){
    nDown = true;
    nDownClicked = true;
  }

  if(n.Z1 == 1 and nZ == false){
    nZ = true;
    nZClicked = true;
  }
  if(n.c1 == 1 and nc == false){
    nc = true;
    ncClicked = true;
  }
}
void ControllerClickedReset(){
  nRightClicked = false;
  nLeftClicked = false;
  
  nUpClicked = false;
  nDownClicked = false;

  ncClicked = false;
  nZClicked = false;
  //Reset left or right clicked for single frame click
  if(n.joyX1 < 30 and n.joyX1 > -30){
    nRight = false;
    nLeft = false;
  }

  if(n.joyY1 < 30 and n.joyY1 > -30){
    nUp = false;
    nDown = false;
  }

  if(n.c1 == 0) nc = false;
  if(n.Z1 == 0) nZ = false;
}

// **************************************************************
// TANKIE GAME
// **************************************************************
unsigned long tankieFrameRate = 18;
unsigned long prevTankieFrameRate = 0;

struct TankStruct {
  double x = 50;
  double y = 50;
  int dirX = 0;
  int dirY = 0;
  double speed = 0.04;
  int up, down, left, right;
  double offset = 17;
};
TankStruct tank;

struct BulletStruct {
  bool fired = false;
  double x = 50;
  double y = 50;
  int dirX = 1;
  int dirY = 0;
  double speed = 4;
};
double bulletOffset = 19/2;

BulletStruct bullet1;
BulletStruct bullet2;
BulletStruct bullet3;
BulletStruct bullet4;
BulletStruct bullet5;
BulletStruct bullet6;

int bulletCount = 6;
BulletStruct bullets[] = {bullet1, bullet2, bullet3, bullet4, bullet5, bullet6};

void Tankie(){
  TankieDir();
  TankieFire();

  int currentTime = millis();
  if(currentTime - prevTankieFrameRate >= tankieFrameRate){
    prevTankieFrameRate = currentTime;

    TankieMove();
    BulletsMove();
  }
}

void TankieFire(){
  if(nZClicked){
    //FIRE
    for(int i = 0; i < bulletCount; i++){
      if(bullets[i].fired == false){
        if(tank.dirX == 0 and tank.dirY == 0) break;

        bullets[i].x = tank.x + bulletOffset;
        bullets[i].y = tank.y + bulletOffset;

        bullets[i].dirX = tank.dirX;
        bullets[i].dirY = tank.dirY;

        bullets[i].fired = true;

        break;
      }
    }
  }

  else if(n.Z2){
    for(int i = 0; i < bulletCount; i++){
      bullets[i].fired = false;
    }
  }
}

void TankieMove(){
  // if((tank.dirX == 1 and tank.dirY == 1) or (tank.dirX == 1 and tank.dirY == -1) or (tank.dirX == -1 and tank.dirY == 1) or (tank.dirX == -1 and tank.dirY == -1)){
  //   tank.x += tank.dirX * tank.speed * ANGLE_CONSTANT;
  //   tank.y -= tank.dirY * tank.speed * ANGLE_CONSTANT;
  // }
  // else{
  //   tank.x += tank.dirX * tank.speed;
  //   tank.y -= tank.dirY * tank.speed;
  // }

  tank.x += n.joyX1 * tank.speed;
  tank.y -= n.joyY1 * tank.speed;

  if(tank.dirX == 1){
    tank.right = 17;
    tank.left = 3;
    tank.up = 3;
    tank.down = 8;
  }
  if(tank.dirX == -1){
    tank.right = 3;
    tank.left = 17;
    tank.up = 3;
    tank.down = 8;
  }
  if(tank.dirY == 1){
    tank.right = 8;
    tank.left = 3;
    tank.up = 17;
    tank.down = 3;
  }
  if(tank.dirY == -1){
    tank.right = 8;
    tank.left = 3;
    tank.up = 3;
    tank.down = 17;
  }

  if(tank.x + tank.offset + tank.right > 160){
    tank.x -= n.joyX1 * tank.speed;
  }
  else if(tank.x + tank.offset - tank.left < 0){
    tank.x -= n.joyX1 * tank.speed;
  }

  if(tank.y + tank.offset - tank.up < 0){
    tank.y += n.joyY1 * tank.speed;
  }
  else if(tank.y + tank.offset + tank.down > 128){
    tank.y += n.joyY1 * tank.speed;
  }
}

void TankieDir(){
  if(n.joyX1 > 30) tank.dirX = 1;
  else if(n.joyX1 < -30) tank.dirX = -1;
  else tank.dirX = 0;

  if(n.joyY1 > 30) tank.dirY = 1;
  else if(n.joyY1 < -30) tank.dirY = -1;
  else tank.dirY = 0;
}

void DrawTankie(){
  //Draw background image
  backgroundSprite.pushImage(0, 0, 160, 128, Level1);

  if(tank.dirX == 1 and tank.dirY == 0) tankSprite.pushImage(0, 0, 34, 34, AceRight);
  else if(tank.dirX == 1 and tank.dirY == 1) tankSprite.pushImage(0, 0, 34, 34, AceRightUp);
  else if(tank.dirX == 1 and tank.dirY == -1) tankSprite.pushImage(0, 0, 34, 34, AceRightDown); 

  else if(tank.dirX == -1 and tank.dirY == 0) tankSprite.pushImage(0, 0, 34, 34, AceLeft);
  else if(tank.dirX == -1 and tank.dirY == 1) tankSprite.pushImage(0, 0, 34, 34, AceLeftUp);
  else if(tank.dirX == -1 and tank.dirY == -1) tankSprite.pushImage(0, 0, 34, 34, AceLeftDown);

  else if(tank.dirX == 0 and tank.dirY == 1) tankSprite.pushImage(0, 0, 34, 34, AceUp);
  else if(tank.dirX == 0 and tank.dirY == -1) tankSprite.pushImage(0, 0, 34, 34, AceDown);

  //Draw tank image
  tankSprite.pushToSprite(&backgroundSprite, tank.x, tank.y, TFT_BLACK);
  // tankSprite.pushToSprite(&backgroundSprite, tank.x, tank.y, TFT_GREEN);

  //Draw Bullets
  for(int i = 0; i < bulletCount; i++){
    if(bullets[i].fired == false) continue;
    backgroundSprite.fillCircle(bullets[i].x, bullets[i].y, 5, TFT_RED);
  }

  backgroundSprite.fillRect(45, 29, 6, 70, TFT_GREEN);

  backgroundSprite.pushSprite(0, 0);
}

void BulletsMove(){
  for(int i = 0; i < bulletCount; i++){
    if(bullets[i].fired == true){
      bullets[i].x += bullets[i].speed * bullets[i].dirX;
      bullets[i].y -= bullets[i].speed * bullets[i].dirY;

      if(bullets[i].x < 0 or bullets[i].x > 160 or bullets[i].y < 0 or bullets[i].y > 128){
        bullets[i].fired = false;
      }
    }
    else{
      bullets[i].x = -20;
    }
  }
}

void setup() {
  Serial.begin(115200);
  InitESPNOW();
  InitTFT();
}

void loop() {
  ControllerClicked();

  // Main Loop
  Tankie();

  DrawTankie();

  ControllerClickedReset();
}

void InitTFT(){
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_PURPLE);

  backgroundSprite.createSprite(160, 128);
  backgroundSprite.setSwapBytes(true);
  tankSprite.createSprite(34, 34);
  tankSprite.pushImage(0, 0, 34, 34, AceRight);
}

void InitESPNOW(){
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);}


void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&n, incomingData, sizeof(n));}