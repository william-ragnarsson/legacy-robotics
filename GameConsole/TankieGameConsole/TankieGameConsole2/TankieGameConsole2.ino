// Includes and definitions
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

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

// **************************************************************
// TANKIE
// **************************************************************
unsigned long tankieFrameRate = 10;
unsigned long prevTankieFrameRate = 0;

float tankX = 50;
float tankY = 50;
int tankRadius = 3;

float bulletX = 50;
float bulletY = 50;
float bulletDirX;
float bulletDirY;
bool shot = false;


void Tankie(){
  int currentTime = millis(); 

  if(currentTime - prevTankieFrameRate >= tankieFrameRate){
    prevTankieFrameRate = currentTime;


    //UPDATE TANK POSITION AND DRAW
    tft.fillCircle(tankX, tankY, tankRadius, TFT_BLACK);
    if(n.joyX1 > 20 or n.joyX1 < -20) tankX += float(n.joyX1)/50;
    if(n.joyY1 > 20 or n.joyY1 < -20) tankY -= float(n.joyY1)/50; 
    tft.fillCircle(tankX, tankY, tankRadius, TFT_WHITE);

    //GET THE ANGLE
    if(shot == false){
      bulletX = tankX;
      bulletY = tankY;
    }
    else{
      
    }

    // tft.drawLine(tankX, tankY, int32_t xe, int32_t ye, uint32_t color)
  }
}

// void GetTankAngle(){
//   // if(n.joyX2 == 0 and n.joyY2 == 0) return;

//   // //PREVENT nan
//   // if(n.joyX2 == 0){
//   //   if(n.joyY2 > 0) angle = 1.57;
//   //   if(n.joyY2 < 0) angle = 4.71;
//   //   return;
//   // }
//   // if(n.joyY2 == 0){
//   //   if(n.joyX2 > 0) angle = 0;
//   //   if(n.joyX2 < 0) angle = 3.14;
//   //   return;
//   // }

//   angle = atan2(double(n.joyY2), double(n.joyX2));
// }

//!!!!!!!!!!!!!!!!!!!!!!!!
//Not needed, foor bullet travel, just move over joyX and joyY positions, don't need to calculate angle, to then turn it back into x and y values

void setup() {
  Serial.begin(115200);
  InitESPNOW();
  InitTFT();
}

void loop() {
  ControllerClicked();

  Tankie();

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