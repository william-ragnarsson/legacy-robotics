//MAC address B8:D6:1A:44:3B:4C
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

// //TEST
// #include "SnakeGameImg5.h"
// #include "SnakeGameImg6.h"
// #include "PongImg2.h"

TFT_eSPI tft = TFT_eSPI();

//ESP NOW structure
typedef struct structMessage {
  bool Z, c;
  int joyX, joyY;
};
structMessage nChuk;

int screenIndex = 0;

//************************************************************************************************
//SETTINGS MENU
//************************************************************************************************

int cursorPos = 0;
int prevCursorPos = -1;
bool settingsUp = false;
bool settingsDown = false;
bool settingsRight = false;
bool settingsLeft = false;

String portal = "on"; 
String prevPortal = portal;

float snakeSpeed = 1.0;
float prevSnakeSpeed = snakeSpeed;

int foodCount = 1;
int prevFoodCount = foodCount;

void SettingsMenu(){
  if(nChuk.Z){
    screenIndex = 0;
    SnakeInit();
    return;
  }

  UpdateSettingsCursor();

  UpdateSettingsText();

  prevCursorPos = cursorPos;

  prevPortal = portal;
  prevSnakeSpeed = snakeSpeed;
  prevFoodCount = foodCount;
}

void UpdateSettingsText(){
  if(cursorPos != prevCursorPos){
    if(prevCursorPos == 0){
      tft.setTextColor(TFT_WHITE);
      tft.drawString("Portal", 10, 40);
      tft.drawString(portal, 110, 40);
    }
    else if(prevCursorPos == 1){
      tft.setTextColor(TFT_WHITE);
      tft.drawString("Speed", 10, 64);
      tft.drawFloat(snakeSpeed, 1, 110, 64);
    }
    else if(prevCursorPos == 2){
      tft.setTextColor(TFT_WHITE);
      tft.drawString("Food", 10, 88);
      tft.drawFloat(foodCount, 0, 110, 88);
    }

    if(cursorPos == 0){
      tft.setTextColor(TFT_BLUE);
      tft.drawString("Portal", 10, 40);
      tft.drawString(portal, 110, 40);
    }
    else if(cursorPos == 1){
      tft.setTextColor(TFT_BLUE);
      tft.drawString("Speed", 10, 64);
      tft.drawFloat(snakeSpeed, 1, 110, 64);
    }
    else if(cursorPos == 2){
      tft.setTextColor(TFT_BLUE);
      tft.drawString("Food", 10, 88);
      tft.drawFloat(foodCount, 0, 110, 88);
    }
  }

  if(portal != prevPortal){
    tft.setTextColor(TFT_BLACK);
    tft.drawString(prevPortal, 110, 40);

    tft.setTextColor(TFT_BLUE);
    tft.drawString("Portal", 10, 40);
    tft.drawString(portal, 110, 40);
  }
  else if(snakeSpeed != prevSnakeSpeed){
    tft.setTextColor(TFT_BLACK);
    tft.drawFloat(prevSnakeSpeed, 2, 110, 64);

    tft.setTextColor(TFT_BLUE);
    tft.drawString("Speed", 10, 64);
    tft.drawFloat(snakeSpeed, 1, 110, 64);
  }
  else if(foodCount != prevFoodCount){
    tft.setTextColor(TFT_BLACK);
    tft.drawFloat(prevFoodCount, 0, 110, 88);

    tft.setTextColor(TFT_BLUE);
    tft.drawString("Food", 10, 88);
    tft.drawFloat(foodCount, 0, 110, 88);
  }
 
}

void UpdateSettingsCursor(){
  //Y-axis
  if(nChuk.joyY > 80 and settingsUp == false){
    cursorPos--;
    if(cursorPos < 0) cursorPos = 2;
    settingsUp = true;
  }

  if(nChuk.joyY < 20 and settingsUp == false){
    cursorPos++;
    if(cursorPos > 2) cursorPos = 0;
    settingsUp = true;
  }

  if(nChuk.joyY > 20 and nChuk.joyY < 80){
    settingsUp = false;
    settingsDown = false;
  }

  //X-axis
  if(nChuk.joyX > 80 and settingsRight == false){
    if(cursorPos == 0){
      if(portal == "on") portal = "off";
      else if(portal == "off") portal = "on";
    }
    else if(cursorPos == 1){
      snakeSpeed += 0.1;
    }
    else if(cursorPos == 2){
      if(foodCount < 5) foodCount++;
    }
  
    settingsRight = true;
  }

  if(nChuk.joyX < 20 and settingsLeft == false){
    if(cursorPos == 0){
      if(portal == "on") portal = "off";
      else if(portal == "off") portal = "on";
    }
    else if(cursorPos == 1){
      if(snakeSpeed > 0.1){
        snakeSpeed -= 0.1;
      }
    }
    else if(cursorPos == 2){
      if(foodCount > 1) foodCount--;
    }

    settingsLeft = true;
  }

  if(nChuk.joyX > 20 and nChuk.joyX < 80){
    settingsRight = false;
    settingsLeft = false;
  }
}

void SettingsInit(){
  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(ML_DATUM);
  tft.setTextSize(2);

  tft.setTextColor(TFT_BLUE);
  tft.drawString("Portal", 10, 40);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Speed", 10, 64);
  tft.drawString("Food", 10, 88);

  tft.setTextColor(TFT_BLUE);
  tft.drawString(portal, 110, 40);
  tft.setTextColor(TFT_WHITE);
  tft.drawFloat(snakeSpeed, 1, 110, 64);
  tft.drawFloat(foodCount, 0, 110, 88);

  cursorPos = 0;
  prevCursorPos = -1;
}

//************************************************************************************************
//SNAKE GAME
//************************************************************************************************
unsigned long snakeFrameRate = 100;
unsigned long prevSnakeFrameRate = 0;

int snakeBody[320][2];
int snakeLen = 3;
int prevSnakeLen = snakeLen;

int snakeDir[2] = {1, 0};
int prevSnakeDir[2] = {1, 0};

int foodPosArr[5][2] = {{5, 6}, {10, 4}, {15, 3}, {12, 11}, {19, 8}};

bool snakeGameOver = false;
bool snakeInit = true;

void SnakeGame(){
  if(nChuk.c){
    screenIndex = 1;
    SettingsInit();
    return;
  }

  if(snakeGameOver){
    //Draw end score on top of everything in bright white
    tft.setTextColor(TFT_WHITE);
    DrawSnakeScore();

    //Restart game
    if(nChuk.Z){
      snakeGameOver = false;
      SnakeInit();
    }
    return;
  }

  int currentTime = millis();

  //Update snakeDir every instance, not bound by framerate
  UpdateSnakeDir();

  if(currentTime - prevSnakeFrameRate >= snakeFrameRate){
    prevSnakeFrameRate = currentTime;

    //Fill last snake segment black
    tft.fillRect(snakeBody[0][0] * 8, snakeBody[0][1] * 8, 8, 8, TFT_BLACK);

    //**************************************

    //Draw score before drawing snake
    DrawSnakeScore();

    //**************************************

    //If there is collision with food
    bool foodCollision = false;
    for(int i = 0; i < foodCount; i++){
      if(snakeBody[snakeLen - 1][0] + snakeDir[0] == foodPosArr[i][0] and snakeBody[snakeLen - 1][1] + snakeDir[1] == foodPosArr[i][1]){
        foodCollision = true;
        //Add segment to snake with current headPos + snakeDir
        snakeBody[snakeLen][0] = snakeBody[snakeLen - 1][0] + snakeDir[0];
        snakeBody[snakeLen][1] = snakeBody[snakeLen - 1][1] + snakeDir[1];

        //Remove current number text
        tft.setTextColor(TFT_BLACK); 
        DrawSnakeScore();
        //Increase snake length by 1
        snakeLen++;
        //Update with new number
        tft.setTextColor(0x10A2);
        DrawSnakeScore();

        //Update food
        UpdateFoodPos();
      }


      //In case screenwrapping is on and food is on the edge of the board
      else if(snakeBody[snakeLen - 1][0] + snakeDir[0] == foodPosArr[i][0] and snakeBody[snakeLen - 1][1] + snakeDir[1] == foodPosArr[i][1]){
        foodCollision = true;
        //Add segment to snake with current headPos + snakeDir
        snakeBody[snakeLen][0] = snakeBody[snakeLen - 1][0] + snakeDir[0];
        snakeBody[snakeLen][1] = snakeBody[snakeLen - 1][1] + snakeDir[1];

        //Remove current number text
        tft.setTextColor(TFT_BLACK); 
        DrawSnakeScore();
        //Increase snake length by 1
        snakeLen++;
        //Update with new number
        tft.setTextColor(0x10A2);
        DrawSnakeScore();

        //Update food
        UpdateFoodPos();
      }
    }
    if(foodCollision == false){
      int prevSnakeTail[2] = {snakeBody[0][0], snakeBody[0][1]};

      //Move each segment one up
      for(int i = 0; i < snakeLen - 1; i++){
        snakeBody[i][0] = snakeBody[i+1][0];
        snakeBody[i][1] = snakeBody[i+1][1];
      }

      //Update Head of the snake
      snakeBody[snakeLen - 1][0] += snakeDir[0];
      snakeBody[snakeLen - 1][1] += snakeDir[1];

      //Check FOR WALL COLLISION
      if(portal == "on"){
        if(snakeBody[snakeLen-1][0] < 0) snakeBody[snakeLen-1][0] += 20;
        else if(snakeBody[snakeLen-1][0] > 19) snakeBody[snakeLen-1][0] -= 20;

        else if(snakeBody[snakeLen-1][1] < 0) snakeBody[snakeLen-1][1] += 16;
        else if(snakeBody[snakeLen-1][1] > 15) snakeBody[snakeLen-1][1] -= 16;
      }
      else if(portal == "off"){
        if(snakeBody[snakeLen-1][0] < 0 or snakeBody[snakeLen-1][1] < 0 or snakeBody[snakeLen-1][0] > 19 or snakeBody[snakeLen-1][1] > 15){
          snakeGameOver = true;
          //Draw last snake segment, purely for visuals
          tft.fillRect(prevSnakeTail[0] * 8, prevSnakeTail[1] * 8, 8, 8, TFT_GREEN);
          return;
        }
      }

      //CHECK FOR BODY COLLISION
      for(int i = 0; i < snakeLen - 1; i++){
        if(snakeBody[snakeLen-1][0] == snakeBody[i][0] and snakeBody[snakeLen-1][1] == snakeBody[i][1] ){
          snakeGameOver = true;
          //Draw last snake segment, purely for visuals
          tft.fillRect(prevSnakeTail[0] * 8, prevSnakeTail[1] * 8, 8, 8, TFT_GREEN);
          return;
        }
      }
    }

    //Update prevSnakeDir for next frame
    prevSnakeDir[0] = snakeDir[0];
    prevSnakeDir[1] = snakeDir[1];

    //Draw food
    for(int i = 0; i < foodCount; i++){
      tft.fillRect(foodPosArr[i][0] * 8, foodPosArr[i][1] * 8, 8, 8, TFT_BLUE);
    }

    //Draw full snake
    for(int i = 0; i < snakeLen; i++){
      tft.fillRect(snakeBody[i][0] * 8, snakeBody[i][1] * 8, 8, 8, TFT_GREEN);
    }
  }
}

void UpdateSnakeDir(){
  //Update snake directions
  //Cant move in opposite direction
  if(nChuk.joyX > 80 and prevSnakeDir[0] != -1){
    snakeDir[0] = 1; snakeDir[1] = 0;
  }
  else if(nChuk.joyX < 20 and prevSnakeDir[0] != 1){
    snakeDir[0] = -1; snakeDir[1] = 0;
  }

  else if(nChuk.joyY > 80 and prevSnakeDir[1] != 1){
    snakeDir[0] = 0; snakeDir[1] = -1;
  }
  else if(nChuk.joyY < 20 and prevSnakeDir[1] != -1){
    snakeDir[0] = 0; snakeDir[1] = 1;
  }
}

void UpdateFoodPos(){
  int foodPosArrIndex = 0;
  for(int i = 0; i < foodCount; i++){
    if(snakeBody[snakeLen-1][0] == foodPosArr[i][0] and snakeBody[snakeLen-1][1] == foodPosArr[i][1]) foodPosArrIndex = i;
  }

  //Set random values for food
  //If it collides with collides with snake, respawn
  bool collision = true;
  while(collision){
    collision = false; //Assume no collision

    foodPosArr[foodPosArrIndex][0] = random(20);
    foodPosArr[foodPosArrIndex][1] = random(16);

    //Check if food collides with snake or with other food
    for(int i = 0; i < snakeLen; i++){
      if(foodPosArr[foodPosArrIndex][0] == snakeBody[i][0] and foodPosArr[foodPosArrIndex][1] == snakeBody[i][1]) collision = true;
      else{
        for(int j = 0; j < foodPosArrIndex; j++){
          if(foodPosArr[foodPosArrIndex][0] == foodPosArr[j][0] and foodPosArr[foodPosArrIndex][1] == foodPosArr[j][1]) collision = true;
        }
        for(int j = foodPosArrIndex + 1; j < foodCount - 1; j++){
          if(foodPosArr[foodPosArrIndex][0] == foodPosArr[j][0] and foodPosArr[foodPosArrIndex][1] == foodPosArr[j][1]) collision = true;
        }
      }
    }
  }
}

void DrawSnakeScore(){
  if(snakeLen > 9 and snakeLen < 20){
    tft.drawString(String(snakeLen), 65, 64, 7);
  }
  else{
    tft.drawString(String(snakeLen), 81, 64, 7);
  }
}

void SnakeInit(){
  //Reset to standard values
  snakeFrameRate = 100 / snakeSpeed;

  snakeLen = 3;
  snakeDir[0] = 1;
  snakeDir[1] = 0;

  for(int i = 0; i < 320; i++){
    snakeBody[i][0] = -1;
    snakeBody[i][1] = -1;
  }

  snakeBody[0][0] = 0; snakeBody[0][1] = 0;
  snakeBody[1][0] = 1; snakeBody[1][1] = 0;
  snakeBody[2][0] = 2; snakeBody[2][1] = 0;

  //Set tft values
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(4);
  tft.setTextColor(0x10A2);

  // //Set food Position
  foodPosArr[0][0] = random(7, 15);
  foodPosArr[0][1] = random(9, 16);

  foodPosArr[1][0] = random(15, 20);
  foodPosArr[1][1] = random(0, 9);

  foodPosArr[2][0] = random(0, 7);
  foodPosArr[2][1] = random(9, 16);

  foodPosArr[3][0] = random(7, 15);
  foodPosArr[3][1] = random(0, 9);

  foodPosArr[4][0] = random(15, 20);
  foodPosArr[4][1] = random(9, 16);
}

void setup() {
  //Init serial monitor
  Serial.begin(115200);

  //Init ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  //Screen INIT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  //TEST
  tft.setSwapBytes(true);

  SettingsInit();
  SnakeInit();
}



void loop() {
  if(screenIndex == 0){
    SnakeGame();
  }
  else if(screenIndex == 1){
    SettingsMenu();
  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&nChuk, incomingData, sizeof(nChuk));
}