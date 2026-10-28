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
  // Set secondary controller to primary if the primary controller isn't connected
  if(n.connected1 == false){n.c1 = n.c2; n.Z1 = n.Z2; n.joyX1 = n.joyX2; n.joyY1 = n.joyY2;}

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

void Snake(){
  if(ncClicked){
    gameConsoleIndex = GCI("SettingsMenu");
    SettingsInit();
    return;
  }

  // Snake game
  tft.fillScreen(TFT_GREEN);
}

// *****************************************************************************
// PONG GAME
// *****************************************************************************

void Pong(){
  if(ncClicked){
    gameConsoleIndex = GCI("SettingsMenu");
    SettingsInit();
    return;
  }

  // Pong game
  tft.fillScreen(TFT_BLUE);
}

// *****************************************************************************
// MAIN MENU
// *****************************************************************************
void MainMenu(){
  if(nZClicked) {
    if(mainMenuIndex == MMI("Snake")){
      gameConsoleIndex = GCI("Snake");
      // SnakeInit();
    }
    else if(mainMenuIndex == MMI("Pong")){
      gameConsoleIndex = GCI("Pong");
      // PongInit();
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
String settingsPongOptions[4] = {"Bounce", "Board", "Ball", "Inverted"};

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
        case 0: tft.drawString(settingsSnakeOptions[0], settingsTextXPos, settingsOptionsYPos[0]); tft.drawString("on", settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawString(settingsSnakeOptions[1], settingsTextXPos, settingsOptionsYPos[1]); tft.drawString("1.0", settingsValueXPos, settingsOptionsYPos[1]); break;
        case 2: tft.drawString(settingsSnakeOptions[2], settingsTextXPos, settingsOptionsYPos[2]); tft.drawString("1", settingsValueXPos, settingsOptionsYPos[2]); break;
        case 3: tft.drawString(settingsSnakeOptions[3], settingsTextXPos, settingsOptionsYPos[3]); tft.drawString("on", settingsValueXPos, settingsOptionsYPos[3]); break;
      }
    }
    else if(mainMenuIndex == MMI("Pong")){
      switch (settingsIndex) {
        case 0: tft.drawString(settingsPongOptions[0], settingsTextXPos, settingsOptionsYPos[0]); tft.drawString("on", settingsValueXPos, settingsOptionsYPos[0]); break;
        case 1: tft.drawString(settingsPongOptions[0], settingsTextXPos, settingsOptionsYPos[1]); tft.drawString("1.0", settingsValueXPos, settingsOptionsYPos[0]); break;
        case 2: tft.drawString(settingsPongOptions[0], settingsTextXPos, settingsOptionsYPos[2]); tft.drawString("1.0", settingsValueXPos, settingsOptionsYPos[0]); break;
        case 3: tft.drawString(settingsPongOptions[0], settingsTextXPos, settingsOptionsYPos[3]); tft.drawString("on", settingsValueXPos, settingsOptionsYPos[0]); break;
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
        case 0: tft.drawString("Portal", 4, 0 + 4); break;
        case 1: tft.drawString("Speed", 4, 16 + 2*4); break;
        case 2: tft.drawString("Count", 4, 32 + 3*4); break;
        case 3: tft.drawString("Inverted", 4, 48 + 4*4); break;
      }
    }
    else if(mainMenuIndex == MMI("Pong")){
      switch (settingsIndex) {
        case 0: tft.drawString("Bounce", 4, 0 + 4); break;
        case 1: tft.drawString("Board", 4, 16 + 2*4); break;
        case 2: tft.drawString("Ball", 4, 32 + 3*4); break;
        case 3: tft.drawString("Inverted", 4, 48 + 4*4); break;
      }
    }
    tft.setTextDatum(BR_DATUM);
    switch (settingsIndex) {
     case 4: tft.drawString("Main Menu", 156, 112 - 4); break;
     case 5: tft.drawString("Continue", 156, 128); break;
    }
  }

  if(!nZClicked) return;
  
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
      // SnakeInit();
      // Snake();
    }
    else if(mainMenuIndex == MMI("Pong")){
      gameConsoleIndex = GCI("Pong");
      // PongInit();
      // Pong();
    }
  }


}

void SettingsInit(){
  settingsIndex = 0;

  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(TL_DATUM);
  if(mainMenuIndex == MMI("Snake")){
    tft.setTextColor(TFT_RED);
    tft.drawString("Portal", 4, 0 + 4);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("Speed", 4, 16 + 2*4);
    tft.drawString("Count", 4, 32 + 3*4);
    tft.drawString("Inverted", 4, 48 + 4*4);
  }
  else if(mainMenuIndex == MMI("Pong")){
    tft.setTextColor(TFT_RED);
    tft.drawString("Bounce", 4, 0 + 4);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("Board", 4, 16 + 2*4);
    tft.drawString("Ball", 4, 32 + 3*4);
    tft.drawString("Inverted", 4, 48 + 4*4);
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