//MAC address B8:D6:1A:44:3B:4C
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

//TEST
#include "SnakeGameImg5.h"
#include "SnakeGameImg6.h"
#include "PongImg2.h"

TFT_eSPI tft = TFT_eSPI();

//ESP NOW structure
typedef struct structMessage {
  bool Z, c;
  int joyX, joyY;
};
structMessage nChuk;

//************************************************************************************************
//MAIN MENU
//************************************************************************************************
// int menuIndex = 0;
// int gameIndex = -1;

// bool menuRight = false;
// bool menuLeft = false;

// void MainMenu(){
//   UpdateMainMenuController();  

//   UpdateMenuGraphics();

//   if(nChuk.Z){
//     gameIndex = menuIndex;
//   }
// }

// void UpdateMainMenuController(){
//   if(nChuk.joyX > 80 and menuRight == false){
//     menuRight = true;
//     menuIndex++;
//   }
//   else if(nChuk.joyX < 60){
//     menuRight = false;
//   }

//   if(nChuk.joyX < 20 and menuLeft == false){
//     menuLeft = true;
//     menuIndex--;
//   }
//   else if(nChuk.joyX > 40){
//     menuLeft = false;
//   }
// }

// void UpdateMenuGraphics(){
//   if(menuIndex == 0){
//     tft.pushImage(0, 0, 160, 128, SnakeGameImg6);
//   }
//   else if(menuIndex == 1){
//     tft.pushImage(0, 0, 160, 128, PongImg2);
//   }
// }

//************************************************************************************************
//SNAKE GAME
//************************************************************************************************
const unsigned long snakeFrameRate = 100;
unsigned long prevSnakeFrameRate = 0;

int snakeBody[320][2];
int snakeLen = 3;
int prevSnakeLen = snakeLen;

int snakeDir[2] = {1, 0};
int prevSnakeDir[2] = {1, 0};

int foodPos[2];

bool gameOver = false;

void SnakeGame(){
  if(nChuk.c){
      gameIndex = -1;
      return;
  }

  if(gameOver){
    //Draw end score on top of everything in bright white
    tft.setTextColor(TFT_WHITE);
    DrawSnakeScore();

    //Restart game
    if(nChuk.Z){
      gameOver = false;
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
    if(snakeBody[snakeLen - 1][0] + snakeDir[0] == foodPos[0] and snakeBody[snakeLen - 1][1] + snakeDir[1] == foodPos[1]){
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
    else{
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
      if(snakeBody[snakeLen-1][0] < 0 or snakeBody[snakeLen-1][1] < 0 or snakeBody[snakeLen-1][0] > 19 or snakeBody[snakeLen-1][1] > 15){
        gameOver = true;
        //Draw last snake segment, purely for visuals
        tft.fillRect(prevSnakeTail[0] * 8, prevSnakeTail[1] * 8, 8, 8, TFT_GREEN);
        return;
      }
      //CHECK FOR BODY COLLISION
      for(int i = 0; i < snakeLen - 1; i++){
        if(snakeBody[snakeLen-1][0] == snakeBody[i][0] and snakeBody[snakeLen-1][1] == snakeBody[i][1] ){
          gameOver = true;
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
    tft.fillRect(foodPos[0] * 8, foodPos[1] * 8, 8, 8, TFT_BLUE);

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
  //Set random values for food
  //If it collides with collides with snake, respawn
  bool collision = true;
  while(collision){
    collision = false; //Assume no collision

    foodPos[0] = random(20);
    foodPos[1] = random(16);

    for(int i = 0; i < snakeLen; i++){
      if(foodPos[0] == snakeBody[i][0] and foodPos[1] == snakeBody[i][1]){
        collision = true;
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

  //Set food Position
  UpdateFoodPos();
}

void setup() {
  //Init serial monitor
  Serial.begin(115200);

  //Init ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  //Screen INIT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  //TEST
  tft.setSwapBytes(true);  
}

void loop() {
  //****************************************************
  //MAIN MENU
  //****************************************************
  // if(gameIndex == -1){
  //   MainMenu();
  // }
  // else if(gameIndex == 0){
  //   SnakeGame();
  // }

  SnakeGame();
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&nChuk, incomingData, sizeof(nChuk));
}