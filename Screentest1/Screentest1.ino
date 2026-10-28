//Display libraries
#include <TFT.h>  
#include <SPI.h>

#define cs   10
#define dc   9
#define rst  8

TFT TFTscreen = TFT(cs, dc, rst);

//Snake game variables
const int displayWidth = 160;
const int displayHeight = 128;

const int red[3] = {0, 0, 255};
const int green[3] = {255, 0, 0};

const int segmentSize = 8;

int snakeX = 8;
int snakeY = 8;

int *snakeBody[2];

int dirX = 1;
int dirY = 0;

int foodX = 0;
int foodY = 0;

//Draw Any Rect
void DrawRect(int x, int y, int color[3]){
  TFTscreen.fillRect(x, y, segmentSize, segmentSize, TFTscreen.Color565(color[0], color[1], color[2]));
}

//Update Food Position
void RandomFoodPos(){
  foodX = random(0, displayWidth);
  foodY = random(0, displayHeight);
  foodX -= foodX % segmentSize;
  foodY -= foodY % segmentSize;
}

//Implement Screen Wrapping
void ScreenWrap(){
  if(snakeX >= displayWidth){
      snakeX -= displayWidth;
  }
  else if(snakeX < 0){
      snakeX += displayWidth;
  }
  else if(snakeY >= displayHeight){
      snakeY -= displayHeight;
  }
  else if(snakeY < 0){
      snakeY += displayHeight;
  }
}


void Input(){
  String input = Serial.readString();
  input.trim();
  
  if(input == "w"){
    dirX = 0;
    dirY = -1;
  }
  if(input == "a"){
    dirX = -1;
    dirY = 0;
  }
  if(input == "s"){
    dirX = 0;
    dirY = 1;
  }
  if(input == "d"){
    dirX = 1;
    dirY = 0;
  }
}

void setup() {
  Serial.begin(9600);
  
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
}

void loop() {
  //Detect Input
  //Input();
  
  //Clear Screen
  TFTscreen.background(0, 0, 0);

  //Food Code
  if(snakeX == foodX and snakeY == foodY){
    const int snakeSegment[2] = {foodX, foodY};
    const int snakeBodyLength = sizeof(snakeBody);
    snakeBody[snakeBodyLength] = snakeSegment;
  }
  
  //Snake Code
  
  snakeX += dirX;
  snakeY += dirY * segmentSize;

  ScreenWrap();
  
  DrawRect(snakeX, snakeY, green);
}
