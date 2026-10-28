// Includes and definitions
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite backgroundSprite = TFT_eSprite(&tft);
TFT_eSprite tankSprite = TFT_eSprite(&tft);

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

// *************************************************
// TETRIS GAME
// *************************************************

int tetromino[3][3] = {{0, 0, 1}, {1, 1, 0}, {0, 0, 1}};

void RotateTetromino(int* block){
  int tempBlock[3][3] = {
    {*block[2][0], *block[1][0], *block[0][0]},
    {*block[2][1], *block[1][1], *block[0][1]},
    {*block[2][2], *block[1][2], *block[0][2]},
  };
}
 
void setup() {
  Serial.begin(115200);

  // InitESPNOW();
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
}

void loop() {
  ControllerClicked();

  // Main Loop
  RotateTetromino(&tetromino);

  ControllerClickedReset();
}


void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&n, incomingData, sizeof(n));}