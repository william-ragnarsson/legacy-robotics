#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();
  
typedef struct StructJoystick {int x, y, s;};
StructJoystick js;

//******************************************************
//SNAKE GAME
//******************************************************
const unsigned long snakeFrameRate = 100;
unsigned long prevSnakeFrameRate = 0;

int snakeMatrix[16][20][2] = {};
bool snakeGameOver = false;

int snakeDir[2] = {1, 0};
int prevSnakeDir[2] = {1, 0};

int snakeHead[2] = {2, 0};

int snakeTail[2] = {0, 0};
int prevTail[2] = {0, 0};

int snakeFoodPos[2];

int snakeLen = 3;

void SnakeGame(){
  //GAMEOVER LOGIC
  if(snakeGameOver){
    //Draw final score on screen
    tft.setTextDatum(4);
    tft.drawString(String(snakeLen), 81, 64, 8);

    if(js.s == 1){
      SnakeInit();
      snakeGameOver = false;
    }
    return;
  }

  int currentTime = millis();
  UpdateSnakeDir();

  //Snake Game code
  if(currentTime - prevSnakeFrameRate >= snakeFrameRate){
    prevSnakeFrameRate = currentTime;
    
    UpdateSnakeHead();

    //Detect collision
    if(snakeHead[0] < 0 or snakeHead[1] < 0 or snakeHead[0] > 19 or snakeHead[1] > 15){
      //WALL COLLISION
      //GAME OVER*****
      snakeGameOver = true;
    }
    else if(snakeMatrix[snakeHead[1]][snakeHead[0]][0] == 100 and snakeMatrix[snakeHead[1]][snakeHead[0]][1] == 100){
      //FOOD COLLISION
      snakeLen++;
      UpdateSnakeFood();
    }
    else if(snakeMatrix[snakeHead[1]][snakeHead[0]][0] != 0 and snakeMatrix[snakeHead[1]][snakeHead[0]][1] != 0){
      //NORMAL SNAKE COLLISION
      //GAME OVER*****
      snakeGameOver = true;
    }
    else{
      UpdateSnakeTail();
    }

    prevSnakeDir[0] = snakeDir[0];
    prevSnakeDir[1] = snakeDir[1];
  }
}

void UpdateSnakeHead(){
  //Update snakeHead position in the matrix
  snakeMatrix[snakeHead[1]][snakeHead[0]][0] = snakeHead[0] + snakeDir[0];
  snakeMatrix[snakeHead[1]][snakeHead[0]][1] = snakeHead[1] + snakeDir[1];

  //Update snakeHead position
  snakeHead[0] += snakeDir[0];
  snakeHead[1] += snakeDir[1];

  //Draw rect at new snakeHead position
  tft.fillRect(snakeHead[0] * 8, snakeHead[1] * 8, 8, 8, TFT_GREEN);
}

void UpdateSnakeTail(){
  //Define a varialbe to remember the previousTail position
  prevTail[0] = snakeTail[0];
  prevTail[1] = snakeTail[1];

  //Draw Blakc rect at previous tail position to remove it from the screen
  tft.fillRect(snakeTail[0] * 8, snakeTail[1] * 8, 8, 8, TFT_BLACK);

  //Copy the snakeTail values over to the matrix, using a pointer system
  memcpy(snakeTail, snakeMatrix[snakeTail[1]][snakeTail[0]], sizeof(snakeTail));

  //Set previous tail position to 0 in the matrix
  snakeMatrix[prevTail[1]][prevTail[0]][0] = 0;
  snakeMatrix[prevTail[1]][prevTail[0]][1] = 0;
}

void UpdateSnakeFood(){
  //Set a random position for the food
  snakeFoodPos[0] = random(20);
  snakeFoodPos[1] = random(16);

  //Update the random food position, if it collides with the snakeBody
  if(snakeMatrix[snakeFoodPos[1]][snakeFoodPos[0]][0] != 0 or snakeMatrix[snakeFoodPos[1]][snakeFoodPos[0]][1] != 0){
    UpdateSnakeFood();  
  }

  //Set the matrix to 100 on the position of the food for recognition
  snakeMatrix[snakeFoodPos[1]][snakeFoodPos[0]][0] = 100;
  snakeMatrix[snakeFoodPos[1]][snakeFoodPos[0]][1] = 100;

  //Draw the food "RED"
  tft.fillRect(snakeFoodPos[0] * 8, snakeFoodPos[1] * 8, 8, 8, TFT_BLUE);
}

void UpdateSnakeDir(){
  if(js.x == 1 and prevSnakeDir[0] != -1){
    snakeDir[0] = 1;
    snakeDir[1] = 0;
  }
  else if(js.x == -1 and prevSnakeDir[0] != 1){
    snakeDir[0] = -1;
    snakeDir[1] = 0;
  }

  else if(js.y == 1 and prevSnakeDir[1] != -1){
    snakeDir[0] = 0;
    snakeDir[1] = 1;
  }

  else if(js.y == -1 and prevSnakeDir[1] != 1){
    snakeDir[0] = 0;
    snakeDir[1] = -1;
  }
}

void SnakeInit(){
  //Reset entire matrix to 0
  memset(snakeMatrix, 0, sizeof(snakeMatrix));

  //Reset snake direction
  snakeDir[0] = 1; snakeDir[1] = 0;
  prevSnakeDir[0] = 1; prevSnakeDir[1] = 0;

  //Reset snakeHead position to 2, 0
  snakeHead[0] = 2;
  snakeHead[1] = 0;

  //Reset snakeTail position to 0, 0
  snakeTail[0] = 0;
  snakeTail[1] = 0;

  //Reset prevTail to snakeTail position
  prevTail[0] = snakeTail[0];
  prevTail[1] = snakeTail[1];

  //Initialize matrix pointer system
  snakeMatrix[0][0][0] = 1; snakeMatrix[0][0][1] = 0;
  snakeMatrix[0][1][0] = 2; snakeMatrix[0][1][1] = 0;

  //Reset the length of the snake to 3
  snakeLen = 3;

  //Reset screen to blank
  tft.fillScreen(TFT_BLACK);

  //Draw Starting positions of snake body
  tft.fillRect(0, 0, 8, 8, TFT_GREEN);
  tft.fillRect(8, 0, 8, 8, TFT_GREEN);
  tft.fillRect(16, 0, 8, 8, TFT_GREEN);

  //Reset snakeFood position
  snakeFoodPos[0] = 0;
  snakeFoodPos[1] = 0;
  //Set snakeFood,
  //if the position of the food collides with the snake, repeat spawn
  while(
  (snakeFoodPos[0] == 0 and snakeFoodPos[1] == 0) or
  (snakeFoodPos[0] == 1 and snakeFoodPos[1] == 0) or
  (snakeFoodPos[0] == 2 and snakeFoodPos[1] == 0) or 
  (snakeFoodPos[0] == 3 and snakeFoodPos[1] == 0)
  ){
    UpdateSnakeFood();
  }
}

void setup() {
  //Setup wifi
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(OnDataRecv);

  //Initialize screen
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  //Init games
  SnakeInit();
}

void loop() {
  //Run snakeGame
  SnakeGame();
}

//Handles data receive
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&js, incomingData, sizeof(js));
}