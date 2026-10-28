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

String GameConsoleIndex = "MainMenu";



void Game1(){
  if(nChuk.c1){
    GameConsoleIndex = "SettingsMenu";
    SettingsMenuInit("SnakeGame");

    return;
  }
  
  tft.fillScreen(TFT_GREEN);
}


int settingsIndex = 0;

void SettingsMenu(String game){
  if(nChuk.Z1) {
    GameConsoleIndex = "MainMenu";
    return;
  }

  if(nChukUpClicked) settingsIndex++;
  else if(nChukDownClicked) settingsIndex--;

  //Item 1

}

void SettingsMenuInit(String game){
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(ML_DATUM);
  tft.setTextSize(2);

  //Item 1
  if(game == "SnakeGame"){
    tft.setTextColor(TFT_RED);
    tft.drawString("Portal", 5, 40);
  }

  tft.setTextColor(TFT_WHITE);
  tft.drawString("Main menu", 5, 64);
  tft.drawString("Continue", 5, 88);
}


int mainMenuIndex = 0;

void MainMenu(){
  if(nChuk.Z1) {
    GameConsoleIndex = "Game1";
    return;
  }

  if(nChukRightClicked) mainMenuIndex++;
  else if(nChukLeftClicked) mainMenuIndex--;

  tft.fillScreen(TFT_RED);
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
  tft.setSwapBytes(true);
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

  //Main Game Console
  if(GameConsoleIndex == "MainMenu"){
    MainMenu();
  }
  else if(GameConsoleIndex == "SettingsMenu"){
    SettingsMenu("SnakeGame");
  }

  else if(GameConsoleIndex == "Game1"){
    Game1();
  }



  nChukRightClicked = false;
  nChukLeftClicked = false;
  
  nChukUpClicked = false;
  nChukDownClicked = false;
  //Reset left or right clicked for single frame click
  if(nChuk.joyX1 < 30 and nChuk.joyX1 > -30){
    nChukRight = false;
    nChukLeft = false;
  }

  if(nChuk.joyY1 < 30 and nChuk.joyY1 > -30){
    nChukUp = false;
    nChukDown = false;
  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&nChuk, incomingData, sizeof(nChuk));
}