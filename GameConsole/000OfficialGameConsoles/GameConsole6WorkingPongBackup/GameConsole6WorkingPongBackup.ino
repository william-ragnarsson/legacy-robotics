//MAC address B8:D6:1A:44:3B:4C
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

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
//SNAKE GAME
//************************************************************************************************
float snakeSpeed = 1.0;

void SnakeGame(){
  if(nChukcClicked){
    gameConsoleIndex = "SettingsMenu";
    SettingsMenuInit("SnakeGame");

    return;
  }
  
  tft.fillScreen(TFT_GREEN);
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

    tft.fillCircle(ballXPos, ballYPos, ballRadius, TFT_BLUE);

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
    if(mainMenuIndex == 0) gameConsoleIndex = "SnakeGame";
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
    tft.fillScreen(TFT_GREEN);
  }
  else if(mainMenuIndex == 1){
    //Pong Main Menu
    tft.fillScreen(TFT_BLUE);
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
      if(mainMenuIndex == 0) gameConsoleIndex = "SnakeGame";
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
    }
    else if(mainMenuIndex == 1){
      gameConsoleIndex = "PongGame";
      PongInit();
    }
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
  // Serial.begin(115200);

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

  PongInit();
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
  } else if(nChuk.c1 == 1 and nChukc == false){
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