//MAC address B8:D6:1A:44:3B:4C
#include "Arduino.h"
#include <esp_now.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
#include <SPI.h>

#define PI 3.14159265358979323846

TFT_eSPI tft = TFT_eSPI();

//---------------------------
//ESP-NOW STUFF
//---------------------------

typedef struct structJs {
  int up;
  int down;
  int right;
  int left;
};
structJs js;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&js, incomingData, sizeof(js));
}

//---------------------------
//Variables

const unsigned long frameRateInterval = 8;
unsigned long previousFrameTime = 0;

String activeScreen = "MainMenu";

//---------------------------
//MainMenu

bool mainMenuInit = true;

bool isLeftClicked;
bool isRightClicked;
bool leftClick;
bool rightClick;

int selectPos = 1;

void MainMenu(){
  if(mainMenuInit){
    tft.fillScreen(TFT_BLACK);
    mainMenuInit = false;
  }

  selectPos += js.right;
  selectPos -= js.left;

  if(selectPos <= 0){
    tft.drawRect(10, 10, 43, 103, TFT_BLUE);
    tft.drawRect(58, 10, 43, 103, TFT_WHITE);
    tft.drawRect(106, 10, 43, 103, TFT_WHITE);
  }
  else if(selectPos == 1){
    tft.drawRect(10, 10, 43, 103, TFT_WHITE);
    tft.drawRect(58, 10, 43, 103, TFT_BLUE);
    tft.drawRect(106, 10, 43, 103, TFT_WHITE);
  }
  else if(selectPos >= 2){
    tft.drawRect(10, 10, 43, 103, TFT_WHITE);
    tft.drawRect(58, 10, 43, 103, TFT_WHITE);
    tft.drawRect(106, 10, 43, 103, TFT_BLUE);
  }

  if(selectPos <= 0 and js.up == 1){
    activeScreen = "AchtungDieKurve";
  }
}


void setup() {
  Serial.begin(115200);

  //ESP STUFF
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(OnDataRecv);

  //Screen init
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  int currentTime = millis();

  if(currentTime - previousFrameTime >= frameRateInterval){
    previousFrameTime = currentTime;

    if(activeScreen == "MainMenu") MainMenu();
    else if(activeScreen == "AchtungDieKurve") AchtungDieKurve();
  }
}

//---------------------------
//Achtung Die Kurve!

typedef struct structKurve{
  float angle = -random(10, 169);

  float posX;
  float posY;
  int intPosX;
  int intPosY;
  int prevPosX;
  int prevPosY;

  float moveSpeed = 0.25;
  int turnSpeed = 3;
  int travelDist = 0;
};

structKurve resetKurve;
structKurve kurve1;

byte matrix[128][160] = {0};

bool gameOver = false;
bool gameOverInit = true;

void AchtungDieKurve(){
  if(js.down == 1){
    activeScreen = "MainMenu";
  }

  if(gameOver){
    GameOver();
    return;
  }

  //Update Angle
  kurve1.angle += js.left * kurve1.turnSpeed;
  kurve1.angle -= js.right * kurve1.turnSpeed;
  //Update Position
  kurve1.posX += cos(kurve1.angle / 360 * PI) * kurve1.moveSpeed;
  kurve1.posY -= sin(kurve1.angle / 360 * PI) * kurve1.moveSpeed;
  
  kurve1.intPosX = int(kurve1.posX);
  kurve1.intPosY = int(kurve1.posY);

  //Check For Collision
  if((kurve1.intPosX != kurve1.prevPosX or kurve1.intPosY != kurve1.prevPosY) and matrix[kurve1.intPosY][kurve1.intPosX] == 1 ){
    GameOver();
  }
  //Check For OutOfBounds
  else if(kurve1.intPosX < 0 or kurve1.intPosY < 0 or kurve1.intPosX > 159 or kurve1.intPosY > 127){
    GameOver();
  }
  //Check For Diagonal Jump
  //Check if going UP
  else if(kurve1.prevPosY - 1 == kurve1.intPosY){
    //CHeck if going RIGHT
    if(kurve1.prevPosX + 1 == kurve1.intPosX      and     matrix[kurve1.intPosY + 1][kurve1.intPosX] == 1 and matrix[kurve1.intPosY][kurve1.intPosX - 1] == 1){
      //GAME OVER
      GameOver();
    }
    else if(kurve1.prevPosX - 1 == kurve1.intPosX     and     matrix[kurve1.intPosY + 1][kurve1.intPosX] == 1 and matrix[kurve1.intPosY][kurve1.intPosX + 1] == 1){
      //GAME OVER
      GameOver();
    }
  }
  //Check if going DOWN
  else if (kurve1.prevPosY + 1 == kurve1.intPosY){

    //CHeck if going RIGHT
    if(kurve1.prevPosX + 1 == kurve1.intPosX     and     matrix[kurve1.intPosY - 1][kurve1.intPosX] == 1 and matrix[kurve1.intPosY][kurve1.intPosX - 1] == 1){
      //GAME OVER
      GameOver();
    }
    //Check if going LEFT
    else if(kurve1.prevPosX - 1 == kurve1.intPosX    and     matrix[kurve1.intPosY - 1][kurve1.intPosX] == 1 and matrix[kurve1.intPosY][kurve1.intPosX + 1] == 1){
      //GAME OVER
      GameOver();
    }
  }

  //Calculate traveling distance for gaps // Logic for gaps
  kurve1.travelDist++;
  if(kurve1.travelDist < 250){
    //Update matrix to 1 at posX, posY
    matrix[kurve1.intPosY][kurve1.intPosX] = 1;
    //Draw Kurve on that pixel
    tft.drawPixel(kurve1.posX, kurve1.posY, TFT_BLUE);
  }
  else if(kurve1.travelDist >= 270){
    kurve1.travelDist = 0;
  }

  //Update Previous Positions at the end
  kurve1.prevPosX = kurve1.intPosX;
  kurve1.prevPosY = kurve1.intPosY;
}

void GameOver(){
  if(gameOverInit){
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_BLUE);
    tft.drawString("GAME OVER", 0, 0, 8);
    
    gameOver = true;
    gameOverInit = false;
  }

  if(js.up == 1){
    gameOver = false;
    gameOverInit = true;
    kurve1 = resetKurve;
    kurve1.angle = -random(10, 169);
    memset(matrix, 0, sizeof(matrix));

    tft.fillScreen(TFT_BLACK);
  }
}