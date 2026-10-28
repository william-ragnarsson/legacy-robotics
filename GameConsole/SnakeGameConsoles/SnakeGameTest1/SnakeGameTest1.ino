#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();

typedef struct StructJoystick {int up, down, right, left;};
StructJoystick js;

const unsigned long snakeFrameRate = 250;
unsigned long prevSnakeFrameRate = 0;


//***************************
//SNAKE GAME
//***************************

bool snakeCollision[128][160];

typedef struct StructSNake{
  int maxSnakeLen = 20;
  int currentSnakeLen = 3;
  byte snakeBody[100][2] = {};
  int prevSnakePos[2] = {-1, -1};

  int snakeSize[2] = {8, 8};

  int snakeDir[2] = {1, 0};
  int prevSnakeDir[2] = {1, 0};
};

StructSNake snake1;

int foodPos[2] = {64, 32};

void SnakeGame(){

  int currentTime = millis();
  UpdateSnakeDir();

  if(currentTime - prevSnakeFrameRate >= snakeFrameRate){
    prevSnakeFrameRate = currentTime;
    
    snake1.prevSnakeDir[0] = snake1.snakeDir[0];
    snake1.prevSnakeDir[1] = snake1.snakeDir[1];

    UpdateSnakePos();
    DrawSnakeBody();
  }
}

void UpdateSnakeDir(){
  if(js.up == 1 and snake1.prevSnakeDir[1] != 1){
    snake1.snakeDir[1] = -1;
    snake1.snakeDir[0] = 0;
  }
  if(js.down == 1 and snake1.prevSnakeDir[1] != -1){
    snake1.snakeDir[1] = 1;
    snake1.snakeDir[0] = 0;
  }

  if(js.right == 1 and snake1.prevSnakeDir[0] != -1) {
    snake1.snakeDir[0] = 1;
    snake1.snakeDir[1] = 0;
  }
  if(js.left == 1 and snake1.prevSnakeDir[0] != 1) {
    snake1.snakeDir[0] = -1;
    snake1.snakeDir[1] = 0;
  }
}

void UpdateSnakePos(){
  snake1.prevSnakePos[0] = snake1.snakeBody[snake1.currentSnakeLen - 1][0];
  snake1.prevSnakePos[1] = snake1.snakeBody[snake1.currentSnakeLen - 1][1];

  if(snake1.snakeBody[0][0] == foodPos[0] and snake1.snakeBody[0][1] == foodPos[1]){
    snake1.snakeBody[snake1.currentSnakeLen][0] = snake1.snakeBody[snake1.currentSnakeLen - 1][0];
    snake1.snakeBody[snake1.currentSnakeLen][1] = snake1.snakeBody[snake1.currentSnakeLen - 1][1];
    snake1.currentSnakeLen++;
  }

  for(int i = snake1.currentSnakeLen-1; i > 0; i--){ 
    if(snake1.snakeBody[i][0] == -1) break;

    snake1.snakeBody[i][0] = snake1.snakeBody[i-1][0];
    snake1.snakeBody[i][1] = snake1.snakeBody[i-1][1];
  }

  snake1.snakeBody[0][0] += snake1.snakeDir[0] * snake1.snakeSize[0];
  snake1.snakeBody[0][1] += snake1.snakeDir[1] * snake1.snakeSize[0];

  if(snakeCollision[snake1.snakeBody[0][1]][snake1.snakeBody[0][0]] == true){
    Serial.println("COLLISIONNN");
  }
}

void DrawSnakeBody(){
  // tft.drawRect(snake1.prevSnakePos[0], snake1.prevSnakePos[1], snake1.snakeSize[0], snake1.snakeSize[1], TFT_BLACK);

  tft.drawRect(foodPos[0], foodPos[1], 8, 8, TFT_GREEN);

  tft.drawRect(snake1.snakeBody[0][0], snake1.snakeBody[0][1], snake1.snakeSize[0], snake1.snakeSize[1], TFT_WHITE);
  snakeCollision[snake1.snakeBody[0][0]][snake1.snakeBody[0][1]] = true;
}

void setup(){
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(OnDataRecv);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  for(int i = 0; i < snake1.maxSnakeLen; i++){
    snake1.snakeBody[i][0] = -1;
  }

  snake1.snakeBody[0][0] = 24;
  snake1.snakeBody[0][1] = 64;

  snake1.snakeBody[1][0] = 16;
  snake1.snakeBody[1][1] = 64;

  snake1.snakeBody[2][0] = 8;
  snake1.snakeBody[2][1] = 64;

  snake1.snakeBody[3][0] = 0;
  snake1.snakeBody[3][1] = 64;
}

void loop(){
  SnakeGame();
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&js, incomingData, sizeof(js));
}