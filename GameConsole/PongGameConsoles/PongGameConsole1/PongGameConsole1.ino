//MAC address B8:D6:1A:44:3B:4C
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

#define PI 3.1415926535 //10 numbers after ","

TFT_eSPI tft = TFT_eSPI();

//ESP NOW structure
typedef struct structMessage {
  bool connected1, connected2;
  bool c1, Z1, c2, Z2;
  int joyX1, joyY1, joyX2, joyY2;
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

String bounceMode = ">"; 
String prevBounceMode = bounceMode;

float ballSpeed = 1.0;
float prevBallSpeed = ballSpeed;

float acceleration = 1.0;
float prevAcceleration = acceleration;

void SettingsMenu(){
  if(nChuk.Z1){
    screenIndex = 0;
    PongInit();
    return;
  }

  UpdateSettingsCursor();

  UpdateSettingsText();

  prevCursorPos = cursorPos;

  prevBounceMode = bounceMode;
  prevBallSpeed = ballSpeed;
  prevAcceleration = acceleration;
}

void UpdateSettingsText(){
  if(cursorPos != prevCursorPos){
    if(prevCursorPos == 0){
      tft.setTextColor(TFT_WHITE);
      tft.drawString("Bounce", 10, 40);
      tft.drawString(bounceMode, 110, 40);
    }
    else if(prevCursorPos == 1){
      tft.setTextColor(TFT_WHITE);
      tft.drawString("Speed", 10, 64);
      tft.drawFloat(ballSpeed, 1, 110, 64);
    }
    else if(prevCursorPos == 2){
      tft.setTextColor(TFT_WHITE);
      tft.drawString("Accel", 10, 88);
      tft.drawFloat(acceleration, 1, 110, 88);
    }

    if(cursorPos == 0){
      tft.setTextColor(TFT_BLUE);
      tft.drawString("Bounce", 10, 40);
      tft.drawString(bounceMode, 110, 40);
    }
    else if(cursorPos == 1){
      tft.setTextColor(TFT_BLUE);
      tft.drawString("Speed", 10, 64);
      tft.drawFloat(ballSpeed, 1, 110, 64);
    }
    else if(cursorPos == 2){
      tft.setTextColor(TFT_BLUE);
      tft.drawString("Accel", 10, 88);
      tft.drawFloat(acceleration, 1, 110, 88);
    }
  }

  if(bounceMode != prevBounceMode){
    tft.setTextColor(TFT_BLACK);
    tft.drawString(prevBounceMode, 110, 40);

    tft.setTextColor(TFT_BLUE);
    tft.drawString("Bounce", 10, 40);
    tft.drawString(bounceMode, 110, 40);
  }
  else if(ballSpeed != prevBallSpeed){
    tft.setTextColor(TFT_BLACK);
    tft.drawFloat(prevBallSpeed, 1, 110, 64);

    tft.setTextColor(TFT_BLUE);
    tft.drawString("Speed", 10, 64);
    tft.drawFloat(ballSpeed, 1, 110, 64);
  }
  else if(acceleration != prevAcceleration){
    tft.setTextColor(TFT_BLACK);
    tft.drawFloat(prevAcceleration, 1, 110, 88);

    tft.setTextColor(TFT_BLUE);
    tft.drawString("Accel", 10, 88);
    tft.drawFloat(acceleration, 1, 110, 88);
  }
 
}

void UpdateSettingsCursor(){
  //Y-axis
  if(nChuk.joyY1 > 40 and settingsUp == false){
    cursorPos--;
    if(cursorPos < 0) cursorPos = 2;
    settingsUp = true;
  }

  if(nChuk.joyY1 < -40 and settingsUp == false){
    cursorPos++;
    if(cursorPos > 2) cursorPos = 0;
    settingsUp = true;
  }

  if(nChuk.joyY1 > -40 and nChuk.joyY1 < 40){
    settingsUp = false;
    settingsDown = false;
  }

  //X-axis
  if(nChuk.joyX1 > 40 and settingsRight == false){
    if(cursorPos == 0){
      if(bounceMode == ">") bounceMode = "}";
      else if(bounceMode == "}") bounceMode = ">";
    }
    else if(cursorPos == 1){
      ballSpeed += 0.1;
    }
    else if(cursorPos == 2){
      acceleration += 0.1;
    }
  
    settingsRight = true;
  }

  if(nChuk.joyX1 < -40 and settingsLeft == false){
    if(cursorPos == 0){
      if(bounceMode == ">") bounceMode = "}";
      else if(bounceMode == "}") bounceMode = ">";
    }
    else if(cursorPos == 1){
      if(ballSpeed > 0.1){
        ballSpeed -= 0.1;
      }
    }
    else if(cursorPos == 2){
      if(acceleration > 0.1) acceleration -= 0.1;
    }

    settingsLeft = true;
  }

  if(nChuk.joyX1 > -40 and nChuk.joyX1 < 40){
    settingsRight = false;
    settingsLeft = false;
  }
}

void SettingsInit(){
  tft.fillScreen(TFT_BLACK);

  tft.setTextDatum(ML_DATUM);
  tft.setTextSize(2);

  tft.setTextColor(TFT_BLUE);
  tft.drawString("Bounce", 10, 40);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Speed", 10, 64);
  tft.drawString("Accel", 10, 88);

  tft.setTextColor(TFT_BLUE);
  tft.drawString(bounceMode, 110, 40);
  tft.setTextColor(TFT_WHITE);
  tft.drawFloat(ballSpeed, 1, 110, 64);
  tft.drawFloat(acceleration, 1, 110, 88);

  cursorPos = 0;
  prevCursorPos = -1;
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
int ballRadius = 2;
bool ballBounced;


void PongGame(){
  if(nChuk.c1){
    screenIndex = 1;
    SettingsInit();
    return;
  }

  if(pongGameOver){
    //DRAW SOMETHING FOR GAME OVER, or whatever

    //Restart game
    if(nChuk.Z1){
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
      if(bounceMode == ">"){
        ballAngle = PI - ballAngle;
      }
      else if(bounceMode == "}"){
        ballAngle = random(-70, 70) * PI / 180;
      }
      
      ballBounced = true;
    }
    if(ballBounced == false and ballXPos + cos(ballAngle) + ballRadius >= 157 and ballYPos - sin(ballAngle) - ballRadius - board2.yPos >= -5 and board2.yPos + board1.height - (ballYPos - sin(ballAngle) + ballRadius) >= -5){
      if(bounceMode == ">"){
        ballAngle = PI - ballAngle;
      }
      else if(bounceMode == "}"){
        ballAngle = random(120 , 210) * PI / 180;
      }
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
  PongInit();
}

void loop() {
  if(nChuk.connected1 == false){
    nChuk.c1 = nChuk.c2;
    nChuk.Z1 = nChuk.Z2;
    nChuk.joyX1 = nChuk.joyX2;
    nChuk.joyY1 = nChuk.joyY2;
  }

  if(screenIndex == 0){
    PongGame();
  }
  else if(screenIndex == 1){
    SettingsMenu();
  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&nChuk, incomingData, sizeof(nChuk));
}