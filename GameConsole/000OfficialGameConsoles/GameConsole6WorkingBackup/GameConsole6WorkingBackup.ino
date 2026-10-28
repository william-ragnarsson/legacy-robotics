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
float snakeSpeed = 0;

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
float ballSpeed = 0;

void PongGame(){
  if(nChukcClicked){
    gameConsoleIndex = "SettingsMenu";
    SettingsMenuInit("PongGame");

    return;
  }

  tft.fillScreen(TFT_WHITE);
}



//************************************************************************************************
//MAIN MENU
//************************************************************************************************
int mainMenuIndex = 0;

void MainMenu(){
  if(nChukZClicked) {
    if(mainMenuIndex == 0) gameConsoleIndex = "SnakeGame";
    else if(mainMenuIndex == 1) gameConsoleIndex = "PongGame";
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
      else if(mainMenuIndex == 1) gameConsoleIndex = "PongGame";
    }
    
    return;
  }
  else if(nChukcClicked) {
    gameConsoleIndex = "MainMenu";    
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
}

void loop() {
  Serial.println(gameConsoleIndex);

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