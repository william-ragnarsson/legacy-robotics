"""
GameConsole v8
Includes full working SnakeGame and PongGame and AchtungDieKurve
Includes Main Menu Pictures for said games
Includes SettingsMenu for both games with 1 setting each (Speed)
"""

//MAC address B8:D6:1A:44:3B:4C
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

#include "SnakeGameLogo2Correct.h"
#include "PongLogoCorrect.h"

TFT_eSPI tft = TFT_eSPI();

//ESP NOW structure
typedef struct structMessage {
  bool connected1, connected2;
  bool c1, Z1, c2, Z2;
  int joyX1, joyY1, joyX2, joyY2;
};
structMessage nChuk;

bool nChukRight = false;
bool nChukLeft = false;
bool nChukRightClicked = false;
bool nChukLeftClicked = false;

bool nChukUp = false;
bool nChukDown = false;
bool nChukUpClicked = false;
bool nChukDownClicked = false;

bool nChukZ = false;
bool nChukc = false;
bool nChukZClicked = false;
bool nChukcClicked = false;

String gameConsoleIndex = "MainMenu";

//************************************************************************************************
//ACHTUNG DIE KURVE GAME
//************************************************************************************************



//************************************************************************************************
//SNAKE GAME
//************************************************************************************************

unsigned long snakeFrameRate = 100;
unsigned long prevSnakeFrameRate = 0;
float snakeSpeed = 1.0;

int snakeBody[320][2];
int snakeLen = 3;
int prevSnakeLen = snakeLen;

int snakeDir[2] = {1, 0};
int prevSnakeDir[2] = {1, 0};

int foodPosArr[5][2] = {{5, 6}, {10, 4}, {15, 3}, {12, 11}, {19, 8}};
int foodCount = 1;

bool snakeGameOver = false;

void SnakeGame(){
  if(nChukcClicked){
    gameConsoleIndex = "SettingsMenu";
    SettingsMenuInit("SnakeGame");

    return;
  }
  
  if(snakeGameOver){
    //Draw end score on top of everything in bright white
    tft.setTextColor(TFT_WHITE);
    DrawSnakeScore();

    //Restart game
    if(nChukZClicked){
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
    // DrawSnakeScore();

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
        // tft.setTextColor(TFT_BLACK); 
        // DrawSnakeScore();
        //Increase snake length by 1
        snakeLen++;
        //Update with new number
        // tft.setTextColor(0x10A2);
        // DrawSnakeScore();

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
        // tft.setTextColor(TFT_BLACK); 
        // DrawSnakeScore();
        //Increase snake length by 1
        snakeLen++;
        //Update with new number
        // tft.setTextColor(0x10A2);
        // DrawSnakeScore();

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
      // if(portal == "on"){
      //   if(snakeBody[snakeLen-1][0] < 0) snakeBody[snakeLen-1][0] += 20;
      //   else if(snakeBody[snakeLen-1][0] > 19) snakeBody[snakeLen-1][0] -= 20;

      //   else if(snakeBody[snakeLen-1][1] < 0) snakeBody[snakeLen-1][1] += 16;
      //   else if(snakeBody[snakeLen-1][1] > 15) snakeBody[snakeLen-1][1] -= 16;
      // }
      // else if(portal == "off"){
        if(snakeBody[snakeLen-1][0] < 0 or snakeBody[snakeLen-1][1] < 0 or snakeBody[snakeLen-1][0] > 19 or snakeBody[snakeLen-1][1] > 15){
          snakeGameOver = true;
          //Draw last snake segment, purely for visuals
          tft.fillRect(prevSnakeTail[0] * 8, prevSnakeTail[1] * 8, 8, 8, TFT_GREEN);
          return;
        }
      // }

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
      tft.fillRect(foodPosArr[i][0] * 8, foodPosArr[i][1] * 8, 8, 8, TFT_RED);
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
  if(nChuk.joyX1 > 20 and prevSnakeDir[0] != -1){
    snakeDir[0] = 1; snakeDir[1] = 0;
  }
  else if(nChuk.joyX1 < -20 and prevSnakeDir[0] != 1){
    snakeDir[0] = -1; snakeDir[1] = 0;
  }

  else if(nChuk.joyY1 > 20 and prevSnakeDir[1] != 1){
    snakeDir[0] = 0; snakeDir[1] = -1;
  }
  else if(nChuk.joyY1 < -20 and prevSnakeDir[1] != -1){
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

  snakeGameOver = false;  
}

//************************************************************************************************
//PONG GAME
//************************************************************************************************
unsigned long pongFrameRate = 10;
unsigned long prevPongFrameRate = 0;

bool pongGameOver = false;
bool pongInit = true;

//PONG board structure
typedef struct structBoard {
  float yPos = 54;
  float yDir;

  int xPos;
  int width = 3;
  int height = 20;
};
structBoard board1;
structBoard board2;

float ballXPos = 50;
float ballYPos = 50;
float ballAngle = 7*PI/8;
float ballSpeed = 1.0;
int ballRadius = 2;
bool ballBounced;

void PongGame(){
  if(nChukcClicked){
    gameConsoleIndex = "SettingsMenu";
    SettingsMenuInit("PongGame");

    return;
  }

  if(pongGameOver){
    //Restart game
    if(nChukZClicked){
      pongGameOver = false;
      PongInit();
    }
    return;
  }

  int currentTime = millis();

  //Update snakeDir every instance, not bound by framerate
  UpdatePongDir();

  if(currentTime - prevPongFrameRate >= pongFrameRate){
    prevPongFrameRate = currentTime;

    //MOVE BOARDS
    if(board1.yPos + board1.yDir > 0 and board1.yPos + board1.yDir + board1.height < 128){
      tft.fillRect(board1.xPos, board1.yPos, board1.width, board1.height, TFT_BLACK);
      board1.yPos += board1.yDir;
      tft.fillRect(board1.xPos, board1.yPos, board1.width, board1.height, TFT_WHITE);
    }

    if(board2.yPos + board2.yDir > 0 and board2.yPos + board2.yDir + board2.height < 128){
      tft.fillRect(board2.xPos, board2.yPos, board2.width, board2.height, TFT_BLACK);
      board2.yPos += board2.yDir;
      tft.fillRect(board2.xPos, board2.yPos, board2.width, board2.height, TFT_WHITE);
    }

    //MOVE BALL
    tft.fillCircle(ballXPos, ballYPos, ballRadius, TFT_BLACK);

    //Bounce against ceiling / floor
    if(ballYPos - sin(ballAngle) - ballRadius <= 0 or ballYPos - sin(ballAngle) + ballRadius >= 127) ballAngle = 2*PI - ballAngle;
    // if(ballXPos + cos(ballAngle) - ballRadius <= 0 or ballXPos + cos(ballAngle) + ballRadius >= 159) ballAngle = PI - ballAngle;

    if(ballBounced == false and ballXPos + cos(ballAngle) - ballRadius <= 3 and ballYPos - sin(ballAngle) - ballRadius - board1.yPos >= -5 and board1.yPos + board1.height - (ballYPos - sin(ballAngle) + ballRadius) >= -5){
      ballAngle = random(-70, 70) * PI / 180;
      ballBounced = true;
    }
    if(ballBounced == false and ballXPos + cos(ballAngle) + ballRadius >= 157 and ballYPos - sin(ballAngle) - ballRadius - board2.yPos >= -5 and board2.yPos + board1.height - (ballYPos - sin(ballAngle) + ballRadius) >= -5){
      ballAngle = random(120 , 210) * PI / 180;
      ballBounced = true;
    }

    if(ballXPos > 50 and ballXPos < 70) ballBounced = false;

    ballXPos += cos(ballAngle) * ballSpeed;
    ballYPos -= sin(ballAngle) * ballSpeed;

    tft.fillCircle(ballXPos, ballYPos, ballRadius, TFT_RED);

    //If game over, just do a return; and set pongGameOver to true
    if(ballXPos < 0 or ballXPos > 159){
      pongGameOver = true;
      return;
    }

  }

  // Serial.print(board1.yPos); Serial.print(" ");
  // Serial.println(board2.yPos);
}

void UpdatePongDir(){

  board1.yDir = nChuk.joyY1;
  board1.yDir /= -25;

  board2.yDir = nChuk.joyY2;
  board2.yDir /= -25;
}

void PongInit(){
  pongGameOver = false;

  ballBounced = false;
  ballXPos = 50;
  ballYPos = 50;

  board1.xPos = 0;
  board2.xPos = 156;
  board1.yPos = 54;
  board2.yPos = 54;
    
  //Reset to standard values

  //Set tft values
  tft.fillScreen(TFT_BLACK);
  // tft.setTextSize(2);
  // tft.setTextDatum(4);
  // tft.setTextColor(0x10A2);
}


//************************************************************************************************
//MAIN MENU
//************************************************************************************************
int mainMenuIndex = 0;

void MainMenu(){
  if(nChukZClicked) {
    if(mainMenuIndex == 0) {
      gameConsoleIndex = "SnakeGame";
      SnakeInit();      
    }
    else if(mainMenuIndex == 1) {
      gameConsoleIndex = "PongGame";
      PongInit();
    }
    return;
  }

  if(nChukRightClicked and mainMenuIndex < 1) {
    mainMenuIndex++;
  }
  else if(nChukLeftClicked and mainMenuIndex > 0) {
    mainMenuIndex--;
  }


  if(mainMenuIndex == 0){
    //Snake Main Menu
    tft.pushImage(0, 0, 160, 128, SnakeGameLogo2Correct);
  }
  else if(mainMenuIndex == 1){
    //Pong Main Menu
    tft.pushImage(0, 0, 160, 128, PongLogoCorrect);
  }
}

//************************************************************************************************
//SETTINGS MENU
//************************************************************************************************
int settingsIndex = 0;
int prevSettingsIndex = settingsIndex;

void SettingsMenu(String game){
  if(nChukZClicked) {
    if(settingsIndex == 1) gameConsoleIndex = "MainMenu";    
    else if(settingsIndex == 2){
      if(mainMenuIndex == 0) {
        gameConsoleIndex = "SnakeGame";
        SnakeInit();
      }
      else if(mainMenuIndex == 1){
        gameConsoleIndex = "PongGame";
        PongInit();
      }
    }
    return;
  }
  else if(nChukcClicked){
    if(mainMenuIndex == 0){
      gameConsoleIndex = "Snakegame";
      SnakeInit();
    }
    else if(mainMenuIndex == 1){
      Serial.println("START PONGGAME VIA C");
      gameConsoleIndex = "PongGame";
      PongInit();
    }

    return;
  }

  if(nChukUpClicked){
    settingsIndex--;
    if(settingsIndex < 0) settingsIndex = 2;
  }
  else if(nChukDownClicked) {
    settingsIndex++;
    if(settingsIndex > 2) settingsIndex = 0;
  }

  if(settingsIndex != prevSettingsIndex){

    if(prevSettingsIndex == 0){
      if(mainMenuIndex == 0){
        tft.setTextColor(TFT_WHITE);
        tft.drawString("Speed", 5, 40);
        tft.drawFloat(snakeSpeed, 1, 110, 40);
      }
      else if(mainMenuIndex == 1){
        tft.setTextColor(TFT_WHITE);
        tft.drawString("Speed", 5, 40);
        tft.drawFloat(ballSpeed, 1, 110, 40);
      }
    }
    else if(prevSettingsIndex == 1){
      tft.setTextColor(TFT_WHITE);
      tft.drawString("Main menu", 5, 64);
    }
    else if(prevSettingsIndex == 2){
      tft.setTextColor(TFT_WHITE);
      tft.drawString("Continue", 5, 88);      
    }

  
    if(settingsIndex == 0){
      if(mainMenuIndex == 0){
        tft.setTextColor(TFT_RED);
        tft.drawString("Speed", 5, 40);
        tft.drawFloat(snakeSpeed, 1, 110, 40);
      }
      else if(mainMenuIndex){
        tft.setTextColor(TFT_RED);
        tft.drawString("Speed", 5, 40);
        tft.drawFloat(ballSpeed, 1, 110, 40);
      }
    }
    else if(settingsIndex == 1){
      tft.setTextColor(TFT_RED);
      tft.drawString("Main menu", 5, 64);
    }
    else if(settingsIndex == 2){
      tft.setTextColor(TFT_RED);
      tft.drawString("Continue", 5, 88);      
    }
  }

  if(settingsIndex == 0 and nChukRightClicked){
    if(mainMenuIndex == 0) {
      tft.setTextColor(TFT_BLACK);
      tft.drawFloat(snakeSpeed, 1, 110, 40);
      snakeSpeed += 0.1;
      tft.setTextColor(TFT_RED);
      tft.drawFloat(snakeSpeed, 1, 110, 40);
    }
    else if(mainMenuIndex == 1) {
      tft.setTextColor(TFT_BLACK);
      tft.drawFloat(ballSpeed, 1, 110, 40);
      ballSpeed += 0.1;
      tft.setTextColor(TFT_RED);
      tft.drawFloat(ballSpeed, 1, 110, 40);
    }
  }
  else if(settingsIndex == 0 and nChukLeftClicked){
    if(mainMenuIndex == 0 and snakeSpeed > 0.1) {
      tft.setTextColor(TFT_BLACK);
      tft.drawFloat(snakeSpeed, 1, 110, 40);
      snakeSpeed -= 0.1;
      tft.setTextColor(TFT_RED);
      tft.drawFloat(snakeSpeed, 1, 110, 40);
    }
    else if(mainMenuIndex == 1 and ballSpeed > 0.1){
      tft.setTextColor(TFT_BLACK);
      tft.drawFloat(ballSpeed, 1, 110, 40);
      ballSpeed -= 0.1;
      tft.setTextColor(TFT_RED);
      tft.drawFloat(ballSpeed, 1, 110, 40);
    }
  }

  prevSettingsIndex = settingsIndex;
}

void SettingsMenuInit(String game){
  settingsIndex = 0;
  prevSettingsIndex = 0;

  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(ML_DATUM);
  tft.setTextSize(2);

  //Item 1
  if(game == "SnakeGame"){
    tft.setTextColor(TFT_RED);
    tft.drawString("Speed", 5, 40);
    tft.drawFloat(snakeSpeed, 1, 110, 40);
  }

  else if(game == "PongGame"){
    tft.setTextColor(TFT_RED);
    tft.drawString("Speed", 5, 40);
    tft.drawFloat(ballSpeed, 1, 110, 40);
  }

  tft.setTextColor(TFT_WHITE);
  tft.drawString("Main menu", 5, 64);
  tft.drawString("Continue", 5, 88);
}

//************************************************************************************************
//MAIN GAME CONSOLE STRUCTURE
//************************************************************************************************

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
  tft.setSwapBytes(true);

  PongInit();
  SnakeInit();
}

void loop() {
  if(nChuk.connected1 == false){nChuk.c1 = nChuk.c2; nChuk.Z1 = nChuk.Z2; nChuk.joyX1 = nChuk.joyX2; nChuk.joyY1 = nChuk.joyY2;}
  //Detection for right and left "click"
  if(nChuk.joyX1 > 30 and nChukRight == false){
    nChukRight = true;
    nChukRightClicked = true;
  } else if(nChuk.joyX1 < -30 and nChukLeft == false){
    nChukLeft = true;
    nChukLeftClicked = true;
  }

  if(nChuk.joyY1 > 30 and nChukUp == false){
    nChukUp = true;
    nChukUpClicked = true;
  } else if(nChuk.joyY1 < -30 and nChukDown == false){
    nChukDown = true;
    nChukDownClicked = true;
  }

  if(nChuk.Z1 == 1 and nChukZ == false){
    nChukZ = true;
    nChukZClicked = true;
  }
  if(nChuk.c1 == 1 and nChukc == false){
    nChukc = true;
    nChukcClicked = true;
  }

  //************************************************************************************************
  //MAIN GAME CONSOLE
  //************************************************************************************************
  if(gameConsoleIndex == "MainMenu"){
    MainMenu();
  }
  else if(gameConsoleIndex == "SettingsMenu"){
    SettingsMenu("SnakeGame");
  }

  else if(gameConsoleIndex == "SnakeGame"){
    SnakeGame();
  }
  else if(gameConsoleIndex == "PongGame"){
    PongGame();
  }




  nChukRightClicked = false;
  nChukLeftClicked = false;
  
  nChukUpClicked = false;
  nChukDownClicked = false;

  nChukcClicked = false;
  nChukZClicked = false;
  //Reset left or right clicked for single frame click
  if(nChuk.joyX1 < 30 and nChuk.joyX1 > -30){
    nChukRight = false;
    nChukLeft = false;
  }

  if(nChuk.joyY1 < 30 and nChuk.joyY1 > -30){
    nChukUp = false;
    nChukDown = false;
  }

  if(nChuk.c1 == 0) nChukc = false;
  if(nChuk.Z1 == 0) nChukZ = false;
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&nChuk, incomingData, sizeof(nChuk));
}