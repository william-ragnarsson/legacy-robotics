#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
 
// These pins will also work for the 1.8" TFT shield
 
//ESP32-WROOM
#define TFT_DC 12 //A0
#define TFT_CS 13 //CS
#define TFT_MOSI 14 //SDA
#define TFT_CLK 27 //SCK
#define TFT_RST 0 
#define TFT_MISO 0
 
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

extern const uint16_t BLACK = 0x0000;
extern const uint16_t WHITE = 0xFFFF;
extern const uint16_t BLUE = 0x001F;
extern const uint16_t RED = 0xF800;
extern const uint16_t GREEN = 0x07E0;


//Time.deltatime
const unsigned long frameRateInterval = 16;
unsigned long previousFrameTime = 0;

//Screen/Menu control
String activeScreen = "Pong";

//Joystick Variable
int8_t jsDir[3] = {0, 0, 0};

//##############################
//PONG PONG PONG PONG PONG PONG
//##############################

uint8_t ballRadius = 2;
double ballPos[3] = {80, 64};
double ballAngle = 5*PI/4;
double speedFactor = 1;

uint8_t boardWith = 2;
uint8_t boardLen1 = 24;
uint8_t boardLen2 = 24;
uint8_t boardOffset1 = 5;
uint8_t boardOffset2 = 160 - boardOffset1 - boardWith;
double boardPos1 = 64-(boardLen1/2);
double boardPos2 = 64-(boardLen2/2);

bool gameOver = false;
bool justGameOver = false;

const unsigned long speedFacterInterval = 1000;
unsigned long previousSpeedFactorTime = 0;

void UpdateBall();
void UpdateBoard1();
void UpdateBoard2();
void ResetPong();

void Pong(){
  if(gameOver){
    if(justGameOver){
      tft.setCursor(0, 0);
      tft.setTextColor(RED);
      tft.setTextSize(2);
      for(int i = 0; i < 13; i++){
        tft.print("GAMEOVER");
      }
    }
    justGameOver = false;
    //Check for reset
    if(jsDir[2] == 1){
      tft.fillScreen(BLACK);

      gameOver = false;
      speedFactor = 1;

      ballAngle = 5/4*PI;
      ballPos[0] = 80;
      ballPos[1] = 64;
      boardLen1 = 25;
      boardLen2 = 25;
      boardPos1 = 64-(boardLen1/2);
      boardPos2 = 64-(boardLen2/2);
    }
    return;
  }

  UpdateBoard1();
  UpdateBoard2();
  UpdateBall();

  //Increase game speed (currently only ballSpeed)
  unsigned long currentTime = millis();
  if(currentTime - previousSpeedFactorTime >= speedFacterInterval){
    previousSpeedFactorTime = currentTime;
    speedFactor *= 1.05;
  }
}

void UpdateBall(){
  tft.fillCircle(ballPos[0], ballPos[1], ballRadius, BLACK);

  ballPos[0] += cos(ballAngle) * speedFactor;
  ballPos[1] -= sin(ballAngle) * speedFactor;

  //TOP BOTTOM COLLISION
  if(ballPos[1] - ballRadius < 0 or ballPos[1] + ballRadius > 127){
    ballPos[0] -= cos(ballAngle) * speedFactor;
    ballPos[1] += sin(ballAngle) * speedFactor;

    ballAngle = 2*PI - ballAngle;
  }
  //LEFT RIGHT COLLISION
  else if(ballPos[0] - ballRadius < 0 or ballPos[0] + ballRadius > 159){
    ballPos[0] -= cos(ballAngle) * speedFactor;
    ballPos[1] += sin(ballAngle) * speedFactor;

    gameOver = true;
    justGameOver = true;
  }
  //LEFT BOARD COLLISION
  else if(ballPos[0] - ballRadius < boardOffset1 + boardWith and ballPos[1] > boardPos1 and ballPos[1] < boardPos1 + boardLen1){
  
    ballPos[0] -= cos(ballAngle) * speedFactor;
    ballPos[1] += sin(ballAngle) * speedFactor;

    double ballBoardDiff = ballPos[1] - boardPos1;

    if(ballBoardDiff < boardLen1/2){
      ballAngle = 7*PI/18 - (5*PI/18*(ballBoardDiff/(boardLen1/2)));
    }
    else{
      ballAngle = -7*PI/18 + (5*PI/18*((boardLen1 - ballBoardDiff)/(boardLen1/2)));
    }
  }
  //RIGHT BOARD COLLISION
  else if(ballPos[0] + ballRadius > boardOffset2 and ballPos[1] > boardPos2 and ballPos[1] < boardPos2 + boardLen2){
    ballPos[0] -= cos(ballAngle) * speedFactor;
    ballPos[1] += sin(ballAngle) * speedFactor;

    double ballBoardDiff = ballPos[1] - boardPos2;

    if(ballBoardDiff < boardLen2/2){
      ballAngle = PI - (7*PI/18 - (5*PI/18*(ballBoardDiff/(boardLen2/2))));
    }
    else{
      ballAngle = PI - (-7*PI/18 + (5*PI/18*((boardLen2 - ballBoardDiff)/(boardLen2/2))));
    }
  }
  
  tft.fillCircle(ballPos[0], ballPos[1], ballRadius, WHITE);
}

void UpdateBoard1(){
  if(boardPos1 + jsDir[1] * speedFactor >= 0 and boardPos1 + boardLen1 + jsDir[1] * speedFactor <= 128){
    tft.fillRect(boardOffset1, boardPos1, boardWith, boardLen1, BLACK);
    boardPos1 += jsDir[1] * speedFactor;
    tft.fillRect(boardOffset1, boardPos1, boardWith, boardLen1, RED);
  }
}

void UpdateBoard2(){
  if(boardPos2 + boardLen2/2 > ballPos[1] and boardPos2-speedFactor >= 0){
    tft.fillRect(boardOffset2, boardPos2, boardWith, boardLen2, BLACK);
    boardPos2 -= speedFactor;
    tft.fillRect(boardOffset2, boardPos2, boardWith, boardLen2, RED);
  }

  else if(boardPos2 + boardLen2/2 < ballPos[1] and boardPos2 + boardLen2+speedFactor <= 128){
    tft.fillRect(boardOffset2, boardPos2, boardWith, boardLen2, BLACK);
    boardPos2 += speedFactor;
    tft.fillRect(boardOffset2, boardPos2, boardWith, boardLen2, RED);
  }
}

//##############################
//FUNCTIONS FUNCTIONS FUNCTIONS
//##############################

//############################
//MAIN LOOPS
//############################

void setup() {
  //Start serial (SERIOUSLY OPTIONAL)
  // while (!Serial);
  // Serial.begin(9600);

  //Initialize screen
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(BLACK);
  tft.setRotation(3);
}

void loop() {
  unsigned long currentTime = millis();
  
  //Active screens
  if(activeScreen == "Tron"){
    if(currentTime - previousFrameTime >= frameRateInterval){
      previousFrameTime = currentTime;
      //Tron();
    }
  }

  else if(activeScreen == "Pong"){
    if(currentTime - previousFrameTime >= frameRateInterval){
      previousFrameTime = currentTime;
      Pong();
    }
  }
}