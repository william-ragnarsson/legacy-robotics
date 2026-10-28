// Includes and definitions
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

#include "Assets/SnakeLogo.h"
#include "Assets/PongLogo.h"

TFT_eSPI tft = TFT_eSPI();

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

// *****************************************************************************
// MAIN FUNCTIONS
// *****************************************************************************

int mainMenuIndex = 0;
int gameConsoleIndex = 0;

int MMI(String index){
  if(index == "Snake") return 0;
  else if(index == "Pong") return 1;
}

int GCI(String index){
  if(index == "MainMenu") return 0;
  else if(index == "SettingsMenu") return 1;
  else if(index == "Snake") return 2;
  else if(index == "Pong") return 3;
}

// *****************************************************************************
// SNAKE GAME
// *****************************************************************************
float snakeSpeed = 1.0;
int snakeFoodCount = 1;
String snakePortal = "on";
String snakeInverted = "off";

unsigned long snakeFrameRate = 100;
unsigned long prevSnakeFrameRate = 0;

bool snakeGameOver = false;

int snakeBody[320][2];
int snakeLen = 3;
int prevSnakeLen = snakeLen;

int snakeDir[2] = {1, 0};
int prevSnakeDir[2] = {1, 0};
int foodPosArr[5][2] = {{5, 6}, {10, 4}, {15, 3}, {12, 11}, {19, 8}};

void Snake(){
  if(ncClicked){
    gameConsoleIndex = GCI("SettingsMenu");
    SettingsInit();
    return;
  }

  if(snakeGameOver){
    //Draw end score on top of everything in bright white
    tft.setTextColor(TFT_WHITE);
    DrawSnakeScore();

    //Restart game
    if(nZClicked){
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

    //If there is collision with food
    bool foodCollision = false;
    for(int i = 0; i < snakeFoodCount; i++){
      if(snakeBody[snakeLen - 1][0] + snakeDir[0] == foodPosArr[i][0] and snakeBody[snakeLen - 1][1] + snakeDir[1] == foodPosArr[i][1]){
        foodCollision = true;
        //Add segment to snake with current headPos + snakeDir
        snakeBody[snakeLen][0] = snakeBody[snakeLen - 1][0] + snakeDir[0];
        snakeBody[snakeLen][1] = snakeBody[snakeLen - 1][1] + snakeDir[1];

        snakeLen++;

        //Update food
        UpdateFoodPos();
      }


      //In case screenwrapping is on and food is on the edge of the board
      else if(snakeBody[snakeLen - 1][0] + snakeDir[0] == foodPosArr[i][0] and snakeBody[snakeLen - 1][1] + snakeDir[1] == foodPosArr[i][1]){
        foodCollision = true;
        //Add segment to snake with current headPos + snakeDir
        snakeBody[snakeLen][0] = snakeBody[snakeLen - 1][0] + snakeDir[0];
        snakeBody[snakeLen][1] = snakeBody[snakeLen - 1][1] + snakeDir[1];

        snakeLen++;

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

      // Check FOR WALL COLLISION
      if(snakePortal == "on"){
        if(snakeBody[snakeLen-1][0] < 0) snakeBody[snakeLen-1][0] += 20;
        else if(snakeBody[snakeLen-1][0] > 19) snakeBody[snakeLen-1][0] -= 20;

        else if(snakeBody[snakeLen-1][1] < 0) snakeBody[snakeLen-1][1] += 16;
        else if(snakeBody[snakeLen-1][1] > 15) snakeBody[snakeLen-1][1] -= 16;
      }
      else if(snakePortal == "off"){
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
    for(int i = 0; i < snakeFoodCount; i++){
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
  Serial.println(snakeInverted);

  if(snakeInverted == "off"){
    if(n.joyX1 > 15 and prevSnakeDir[0] != -1){
      snakeDir[0] = 1; snakeDir[1] = 0;
    }
    else if(n.joyX1 < -15 and prevSnakeDir[0] != 1){
      snakeDir[0] = -1; snakeDir[1] = 0;
    }

    else if(n.joyY1 > 15 and prevSnakeDir[1] != 1){
      snakeDir[0] = 0; snakeDir[1] = -1;
    }
    else if(n.joyY1 < -15 and prevSnakeDir[1] != -1){
      snakeDir[0] = 0; snakeDir[1] = 1;
    }    
  }
  else if(snakeInverted == "on"){
    if(n.joyX1 > 15 and prevSnakeDir[0] != 1){
      snakeDir[0] = -1; snakeDir[1] = 0;
    }
    else if(n.joyX1 < -15 and prevSnakeDir[0] != -1){
      snakeDir[0] = 1; snakeDir[1] = 0;
    }

    else if(n.joyY1 > 15 and prevSnakeDir[1] != -1){
      snakeDir[0] = 0; snakeDir[1] = 1;
    }
    else if(n.joyY1 < -15 and prevSnakeDir[1] != 1){
      snakeDir[0] = 0; snakeDir[1] = -1;
    }
  }
}

void UpdateFoodPos(){
  int foodPosArrIndex = 0;
  for(int i = 0; i < snakeFoodCount; i++){
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
        for(int j = foodPosArrIndex + 1; j < snakeFoodCount - 1; j++){
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

  snakeBody[0][0] = 0; snakeBody[0][1] = 8;
  snakeBody[1][0] = 1; snakeBody[1][1] = 8;
  snakeBody[2][0] = 2; snakeBody[2][1] = 8;

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

// *****************************************************************************
// PONG GAME
// *****************************************************************************


String pongBallBounce = "{";
String pongPortal = "off";
String pongInverted = "off";
float pongBallSpeed = 1.0;

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
int ballRadius = 2;
bool ballBounced;

void Pong(){
  if(ncClicked){
    gameConsoleIndex = GCI("SettingsMenu");
    SettingsInit();
    return;
  }

  if(pongGameOver){
    //Restart game
    if(nZClicked){
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
    if(pongPortal == "on"){
      if(ballYPos - sin(ballAngle) - ballRadius <= 0) ballYPos += 127;
      else if(ballYPos - sin(ballAngle) + ballRadius >= 127) ballYPos -= 127;
    }
    else if(ballYPos - sin(ballAngle) - ballRadius <= 0 or ballYPos - sin(ballAngle) + ballRadius >= 127){
      ballAngle = 2*PI - ballAngle;
    }    

    if(ballBounced == false and ballXPos + cos(ballAngle) - ballRadius <= 3 and ballYPos - sin(ballAngle) - ballRadius - board1.yPos >= -5 and board1.yPos + board1.height - (ballYPos - sin(ballAngle) + ballRadius) >= -5){
      ballAngle = random(-70, 70) * PI / 180;
      ballBounced = true;
    }
    if(ballBounced == false and ballXPos + cos(ballAngle) + ballRadius >= 157 and ballYPos - sin(ballAngle) - ballRadius - board2.yPos >= -5 and board2.yPos + board1.height - (ballYPos - sin(ballAngle) + ballRadius) >= -5){
      ballAngle = random(120 , 210) * PI / 180;
      ballBounced = true;
    }

    if(ballXPos > 50 and ballXPos < 70) ballBounced = false;

    ballXPos += cos(ballAngle) * pongBallSpeed;
    ballYPos -= sin(ballAngle) * pongBallSpeed;

    tft.fillCircle(ballXPos, ballYPos, ballRadius, TFT_RED);

    //If game over, just do a return; and set pongGameOver to true
    if(ballXPos < 0 or ballXPos > 159){
      pongGameOver = true;
      return;
    }

  }
}

void UpdatePongDir(){
  board1.yDir = n.joyY1;
  board1.yDir /= -25;

  board2.yDir = n.joyY2;
  board2.yDir /= -25;

  if(n.connected2 == false){
    board2.yDir = n.joyY1;
    board2.yDir /= -25;
  }
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
  
  tft.fillScreen(TFT_BLACK);
}

// *****************************************************************************
// MAIN MENU
// *****************************************************************************
void MainMenu(){
  if(nZClicked) {
    if(mainMenuIndex == MMI("Snake")){
      gameConsoleIndex = GCI("Snake");
      SnakeInit();
    }
    else if(mainMenuIndex == MMI("Pong")){
      gameConsoleIndex = GCI("Pong");
      PongInit();
    }
    return;
  }

  if(nRightClicked and mainMenuIndex < 1) mainMenuIndex++;
  else if(nLeftClicked and mainMenuIndex > 0) mainMenuIndex--;

  if(mainMenuIndex == MMI("Snake")){
    // Snake Main Menu screen
    tft.pushImage(0, 0, 160, 128, SnakeLogo);
  }
  else if(mainMenuIndex == MMI("Pong")){
    tft.pushImage(0, 0, 160, 128, PongLogo);
  }
}

// *****************************************************************************
// SETTINGS MENU
// *****************************************************************************
int settingsIndex = 0;

String settingsSnakeOptions[4] = {"Portal", "Speed", "Count", "Inverted"};
String settingsPongOptions[4] = {"Bounce", "Speed", "Inverted", "Portal"};

int settingsOptionsYPos[4] = {4, 24, 44, 64};
int settingsValueXPos = 120;
int settingsTextXPos = 4;

void SettingsMenu(){
  tft.setTextSize(2);
  
  if((nUpClicked and settingsIndex > 0) or (nDownClicked and settingsIndex < 5)){
    //Remove selected before changing color
    tft.setTextColor(TFT_YELLOW);
    tft.setTextDatum(TL_DATUM);
    if(mainMenuIndex == MMI("Snake")){
      switch (settingsIndex) {
        case 0: tft.drawString(settingsSnakeOptions[0], settingsTextXPos, settingsOptionsYPos[0]); tft.drawString(snakePortal, settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawString(settingsSnakeOptions[1], settingsTextXPos, settingsOptionsYPos[1]); tft.drawFloat(snakeSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawString(settingsSnakeOptions[2], settingsTextXPos, settingsOptionsYPos[2]); tft.drawFloat(snakeFoodCount, 0, settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(settingsSnakeOptions[3], settingsTextXPos, settingsOptionsYPos[3]); tft.drawString(snakeInverted, settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
    else if(mainMenuIndex == MMI("Pong")){
      switch (settingsIndex) {
        case 0: tft.drawString(settingsPongOptions[0], settingsTextXPos, settingsOptionsYPos[0]); tft.drawString(pongBallBounce, settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawString(settingsPongOptions[1], settingsTextXPos, settingsOptionsYPos[1]); tft.drawFloat(pongBallSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawString(settingsPongOptions[2], settingsTextXPos, settingsOptionsYPos[2]); tft.drawString(pongInverted, settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(settingsPongOptions[3], settingsTextXPos, settingsOptionsYPos[3]); tft.drawString(pongPortal, settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
    
    tft.setTextColor(TFT_GREENYELLOW);
    tft.setTextDatum(BR_DATUM);
    switch (settingsIndex) {
     case 4: tft.drawString("Main Menu", 156, 112 - 4); break;
     case 5: tft.drawString("Continue", 156, 128); break;
    }

    if(nUpClicked) settingsIndex -= 1;
    else if(nDownClicked) settingsIndex += 1;

    tft.setTextColor(TFT_RED);
    tft.setTextDatum(TL_DATUM);
    // Update selected
    if(mainMenuIndex == MMI("Snake")){
      switch (settingsIndex) {
        case 0: tft.drawString(settingsSnakeOptions[0], settingsTextXPos, settingsOptionsYPos[0]); tft.drawString(snakePortal, settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawString(settingsSnakeOptions[1], settingsTextXPos, settingsOptionsYPos[1]); tft.drawFloat(snakeSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawString(settingsSnakeOptions[2], settingsTextXPos, settingsOptionsYPos[2]); tft.drawFloat(snakeFoodCount, 0, settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(settingsSnakeOptions[3], settingsTextXPos, settingsOptionsYPos[3]); tft.drawString(snakeInverted, settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
    else if(mainMenuIndex == MMI("Pong")){
      switch (settingsIndex) {
        case 0: tft.drawString(settingsPongOptions[0], settingsTextXPos, settingsOptionsYPos[0]); tft.drawString(pongBallBounce, settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawString(settingsPongOptions[1], settingsTextXPos, settingsOptionsYPos[1]); tft.drawFloat(pongBallSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawString(settingsPongOptions[2], settingsTextXPos, settingsOptionsYPos[2]); tft.drawString(pongInverted, settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(settingsPongOptions[3], settingsTextXPos, settingsOptionsYPos[3]); tft.drawString(pongPortal, settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
    tft.setTextDatum(BR_DATUM);
    switch (settingsIndex) {
     case 4: tft.drawString("Main Menu", 156, 112 - 4); break;
     case 5: tft.drawString("Continue", 156, 128); break;
    }
  }
  
  if(nRightClicked){
    tft.setTextDatum(TL_DATUM);
    if(mainMenuIndex == MMI("Snake")){
      tft.setTextColor(TFT_BLACK);
      switch (settingsIndex) {
        case 0: tft.drawString(snakePortal, settingsValueXPos, settingsOptionsYPos[0]); snakePortal = snakePortal == "on" ? "off" : "on";  break;
        case 1: tft.drawFloat(snakeSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); snakeSpeed += 0.1;  break;
        case 2: tft.drawFloat(snakeFoodCount, 0, settingsValueXPos, settingsOptionsYPos[2]); snakeFoodCount = snakeFoodCount < 5 ? snakeFoodCount + 1 : snakeFoodCount;  break;
        case 3: tft.drawString(snakeInverted, settingsValueXPos, settingsOptionsYPos[3]); snakeInverted = snakeInverted == "on" ? "off" : "on";  break;
      }
      tft.setTextColor(TFT_RED);
      switch (settingsIndex) {
        case 0: tft.drawString(snakePortal, settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawFloat(snakeSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawFloat(snakeFoodCount, 0, settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(snakeInverted, settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
    else if(mainMenuIndex == MMI("Pong")){
      tft.setTextColor(TFT_BLACK);
      switch (settingsIndex) {
        case 0: tft.drawString(pongBallBounce, settingsValueXPos, settingsOptionsYPos[0]); pongBallBounce = pongBallBounce == ">" ? "}" : ">"; break;
        case 1: tft.drawFloat(pongBallSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); pongBallSpeed += 0.1; break;
        case 2: tft.drawString(pongInverted, settingsValueXPos, settingsOptionsYPos[2]); pongInverted = pongInverted == "on" ? "off" : "on"; break;
        case 3: tft.drawString(pongPortal, settingsValueXPos, settingsOptionsYPos[3]); pongPortal = pongPortal == "on" ? "off" : "on"; break;
      }
      tft.setTextColor(TFT_RED);
      switch (settingsIndex) {
        case 0: tft.drawString(pongBallBounce, settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawFloat(pongBallSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawString(pongInverted, settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(pongPortal, settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
  }

  if(nLeftClicked){
    tft.setTextDatum(TL_DATUM);
    if(mainMenuIndex == MMI("Snake")){
      tft.setTextColor(TFT_BLACK);
      switch (settingsIndex) {
        case 0: tft.drawString(snakePortal, settingsValueXPos, settingsOptionsYPos[0]); snakePortal = snakePortal == "on" ? "off" : "on"; break;
        case 1: tft.drawFloat(snakeSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); snakeSpeed = snakeSpeed > 0.1 ? snakeSpeed - 0.1 : snakeSpeed; break;
        case 2: tft.drawFloat(snakeFoodCount, 0, settingsValueXPos, settingsOptionsYPos[2]); snakeFoodCount = snakeFoodCount > 1 ? snakeFoodCount - 1 : snakeFoodCount; break;
        case 3: tft.drawString(snakeInverted, settingsValueXPos, settingsOptionsYPos[3]); snakeInverted = snakeInverted == "on" ? "off" : "on"; break;
      }
      tft.setTextColor(TFT_RED);
      switch (settingsIndex) {
        case 0: tft.drawString(snakePortal, settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawFloat(snakeSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawFloat(snakeFoodCount, 0, settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(snakeInverted, settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
    else if(mainMenuIndex == MMI("Pong")){
      tft.setTextColor(TFT_BLACK);
      switch (settingsIndex) {
        case 0: tft.drawString(pongBallBounce, settingsValueXPos, settingsOptionsYPos[0]); pongBallBounce = pongBallBounce == ">" ? "}" : ">"; break;
        case 1: tft.drawFloat(pongBallSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); pongBallSpeed = pongBallSpeed > 0.1 ? pongBallSpeed - 0.1 : pongBallSpeed; break;
        case 2: tft.drawString(pongInverted, settingsValueXPos, settingsOptionsYPos[2]); pongInverted = pongInverted == "on" ? "off" : "on"; break;
        case 3: tft.drawString(pongPortal, settingsValueXPos, settingsOptionsYPos[3]); pongPortal = pongPortal == "on" ? "off" : "on"; break;
      }
      tft.setTextColor(TFT_RED);
      switch (settingsIndex) {
        case 0: tft.drawString(pongBallBounce, settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawFloat(pongBallSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawString(pongInverted, settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(pongPortal, settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
  }

  if(ncClicked){
    if(mainMenuIndex == MMI("Snake")){
      gameConsoleIndex = GCI("Snake");
      SnakeInit();
    }
    else if(mainMenuIndex == MMI("Pong")){
      gameConsoleIndex = GCI("Pong");
      PongInit();
    }
    return;
  }

  if(nZClicked){
    if(mainMenuIndex == MMI("Snake")){
      switch (settingsIndex) {
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
      }
    }
    else if(mainMenuIndex == MMI("Pong")){
      switch (settingsIndex) {
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
      }
    }

    if(settingsIndex == 4){
      gameConsoleIndex = GCI("MainMenu");
    }
    else if(settingsIndex == 5){
      if(mainMenuIndex == MMI("Snake")){
        gameConsoleIndex = GCI("Snake");
        SnakeInit();
      } 
      else if(mainMenuIndex == MMI("Pong")){
        gameConsoleIndex = GCI("Pong");
        PongInit();
      } 
    }

    return;
  }
}

void SettingsInit(){
  settingsIndex = 0;

  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(TL_DATUM);
  if(mainMenuIndex == MMI("Snake")){
    tft.setTextColor(TFT_RED);
    tft.drawString(settingsSnakeOptions[0], settingsTextXPos, settingsOptionsYPos[0]); tft.drawString(snakePortal, settingsValueXPos, settingsOptionsYPos[0]);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString(settingsSnakeOptions[1], settingsTextXPos, settingsOptionsYPos[1]); tft.drawFloat(snakeSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]);
    tft.drawString(settingsSnakeOptions[2], settingsTextXPos, settingsOptionsYPos[2]); tft.drawFloat(snakeFoodCount, 0, settingsValueXPos, settingsOptionsYPos[2]);
    tft.drawString(settingsSnakeOptions[3], settingsTextXPos, settingsOptionsYPos[3]); tft.drawString(snakeInverted, settingsValueXPos, settingsOptionsYPos[3]);
  }
  else if(mainMenuIndex == MMI("Pong")){    
    tft.setTextColor(TFT_RED);
    tft.drawString(settingsPongOptions[0], settingsTextXPos, settingsOptionsYPos[0]); tft.drawString(pongBallBounce, settingsValueXPos, settingsOptionsYPos[0]);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString(settingsPongOptions[1], settingsTextXPos, settingsOptionsYPos[1]); tft.drawFloat(pongBallSpeed, 1, settingsValueXPos, settingsOptionsYPos[1]);
    tft.drawString(settingsPongOptions[2], settingsTextXPos, settingsOptionsYPos[2]); tft.drawString(pongInverted, settingsValueXPos, settingsOptionsYPos[2]);
    tft.drawString(settingsPongOptions[3], settingsTextXPos, settingsOptionsYPos[3]); tft.drawString(pongPortal, settingsValueXPos, settingsOptionsYPos[3]);
  }

  //Draw Return functions
  tft.setTextColor(TFT_GREENYELLOW);
  tft.setTextDatum(BR_DATUM);
  tft.drawString("Main Menu", 156, 112 - 4);
  tft.drawString("Continue", 156, 128);
}

void setup() {
  Serial.begin(115200);
  InitESPNOW();
  InitTFT();

  SettingsInit();
}

void loop() {
  ControllerClicked();

  if(gameConsoleIndex == GCI("MainMenu")) MainMenu();
  else if(gameConsoleIndex == GCI("SettingsMenu")) SettingsMenu();
  else if(gameConsoleIndex == GCI("Snake")) Snake();
  else if(gameConsoleIndex == GCI("Pong")) Pong();

  ControllerClickedReset();
}

void InitESPNOW(){
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);}
void InitTFT(){
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.setTextSize(2);}
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&n, incomingData, sizeof(n));}