// Includes and definitions
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

#include "BirdDown.h"
#include "BottomPipe.h"
#include "TopPipe.h"
#include "Background.h"
#include "Ground.h"

#include "GameOver.h"
#include "FlappyLogo.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite backgroundSprite = TFT_eSprite(&tft);
TFT_eSprite birdSprite = TFT_eSprite(&tft);

// **************************************************************
// CONTROLLER INPUT
// **************************************************************

struct structNunChuk {
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
// FLAPPY BIRD
// **************************************************************
unsigned long flappyFrameRate = 10;
unsigned long prevFlappyFrameRate = 0;

bool flappyGameOver = false;

struct structBird{
  double g = 25;
  double v = 0;
  double v0 = 0;
  double t = 0;
  double jump = 3;

  double y = 128/2;
  double x = 160/3;

  int width = 17;
  int height = 13;
};
structBird flappy;

int pipeWidth = 26;
int pipeSpace = 40;
float pipeSpeed = 0.7;

struct structPipe{
  float x;
  int y;
};
structPipe pipe1;
structPipe pipe3;
structPipe pipe4;
structPipe pipe5;

int pipeArrLen = 3;
structPipe pipes[] = {pipe1, pipe3, pipe4, pipe5};

float groundPos = 0;

void FlappyGame(){
  if(flappyGameOver){
    if(nZClicked){
      FlappyInit();
      flappyGameOver = false;
    }
    return;
  }

  if(nZClicked or ncClicked or nUpClicked){
    flappy.v = 0;
    flappy.v0 = -flappy.jump;
    flappy.t = 0;
  }

  int currentTime = millis(); 
  if(currentTime - prevFlappyFrameRate >= flappyFrameRate){
    prevFlappyFrameRate = currentTime;
    
    UpdateFlappy();
    UpdatePipes();
    Collision();
  }
}

void UpdateFlappy(){
  // tft.fillRect(flappy.x, flappy.y, flappy.width, flappy.height, TFT_BLACK);
  // tft.fillRect(flappy.x, flappy.y, 17, 12, TFT_BLACK);

  flappy.t += double(flappyFrameRate)/1000;
  flappy.v = flappy.v0 + flappy.g * flappy.t;
  flappy.y += flappy.v;

  // tft.fillRect(flappy.x, flappy.y, flappy.width, flappy.height, TFT_RED);
  birdSprite.pushImage(0, 0, 17, 13, BirdDown);
}

void UpdatePipes(){
  for(int i = 0; i < pipeArrLen; i++){
    // DrawPipe(pipes[i].x, pipes[i].y);
    // RemovePipe(pipes[i].x, pipes[i].y);

    pipes[i].x -= pipeSpeed;
    if(pipes[i].x < -160/pipeArrLen) {
      pipes[i].x = 160;
      pipes[i].y = random(8, 80);
    }

    DrawPipe(pipes[i].x, pipes[i].y);
  }
}

// void RemovePipe(int x, int y){
//   screen.fillRect(x, 0, pipeWidth, y, TFT_BLACK);
//   tft.fillRect(x, y + pipeSpace, pipeWidth, 160 - (y + pipeSpace), TFT_BLACK);
// }

void DrawPipe(int x, int y){
  backgroundSprite.pushImage(x, y-84, 26, 84, TopPipe);
  // tft.fillRect(x, 0, pipeWidth, y, color);
  backgroundSprite.pushImage(x, y + pipeSpace, 26, 84, BottomPipe);
  // tft.fillRect(x, y + pipeSpace, pipeWidth, 160 - (y + pipeSpace), color);
}

void Collision(){
  if(flappy.y <= 0 or flappy.y + flappy.height >= 127){
    flappyGameOver = true;
    return;
  }

  for(int i = 0; i < pipeArrLen; i++){
    if(pipes[i].x + pipeWidth < flappy.x or pipes[i].x > flappy.x + flappy.width) continue;

    if(flappy.y <= pipes[i].y){
      flappyGameOver = true;
      return;
    }
    else if(flappy.y + flappy.height >= pipes[i].y + pipeSpace){
      flappyGameOver = true;
      return;
    }
  }
}

void FlappyInit(){
  for(int i = 0; i < pipeArrLen; i++){
    pipes[i].x = 160 + i * 160/(pipeArrLen - 1);
    pipes[i].y = random(14, 61);
  }

  flappy.y = 128/2;
  flappy.x = 160/3;

  flappy.v = 0;
  flappy.v0 = 0;
  flappy.t = 0;

  // tft.fillScreen(TFT_BLACK);
}



void setup() {
  // Serial.begin(115200);

  InitESPNOW();
  InitTFT();

  FlappyInit();
}

void loop() {
  backgroundSprite.pushImage(0, 0, 160, 128, Background);
  

  ControllerClicked();
  FlappyGame();
  ControllerClickedReset();

  birdSprite.pushToSprite(&backgroundSprite, flappy.x, flappy.y, TFT_BLACK);

  backgroundSprite.pushImage(int(groundPos), 115, 166, 13, Ground);
  groundPos -= pipeSpeed;
  if(groundPos <= -5) groundPos = 0;
  backgroundSprite.pushSprite(0, 0, TFT_BLACK);

}

void InitESPNOW(){
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void InitTFT(){
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);

  backgroundSprite.createSprite(160, 128);
  backgroundSprite.setSwapBytes(true);

  birdSprite.createSprite(17, 12);
  // birdSprite.setSwapBytes(true);


}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&n, incomingData, sizeof(n));
}