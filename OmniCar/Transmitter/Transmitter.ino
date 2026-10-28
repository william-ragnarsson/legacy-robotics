//ESP NOW import
#include <ESP8266WiFi.h>
#include <espnow.h>
//ESP NOW nintendo
#include <Wire.h>
#include <NintendoExtensionCtrl.h>

#define SCL1 5
#define SDA1 4

#define SCL2 0
#define SDA2 2

#define PI 3.14159265

uint8_t broadcastAddress[] = { 0x10, 0x91, 0xA8, 0x35, 0x20, 0xE4 };

void ESPInit(){
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

ClassicController controller;

struct struct_message {
  bool connected;
  int dir;
};
struct_message structController;

bool dUp, dDown, dLeft, dRight, a, b, x, y, ZL, ZR, selectB, home, start;
double JXL, JYL, JXR, JYR, L, R;

double angle;

void setup() {
  Serial.begin(115200);  

  ESPInit();
}

void loop() {
  
  Wire.begin(SDA1, SCL1);
  if (!controller.connect()) {
    Serial.println("#1 NOT CONNECTED!");

    structController.connected = false;
  }
  else{
    structController.connected = true;

    controller.update();

    a = controller.buttonA();
    b = controller.buttonB();
    x = controller.buttonX();
    y = controller.buttonY();

    dUp = controller.dpadUp();
    dDown = controller.dpadDown();
    dLeft = controller.dpadLeft();
    dRight = controller.dpadRight();

    ZL = controller.buttonZL();
    ZR = controller.buttonZR();

    selectB = controller.buttonSelect();
    home = controller.buttonHome();
    start = controller.buttonStart();

    JXL = map(controller.leftJoyX(), 30, 232, -90, 90);
    if(JXL < -90) JXL = -90;
    else if (JXL > 90) JXL = 90;
    JYL = map(controller.leftJoyY(), 30, 236, -90, 90);
    if(JYL < -90) JYL = -90;
    else if (JYL > 90) JYL = 90;

    JXR = map(controller.rightJoyX(), 17, 220, -90, 90);
    if(JXR < -90) JXR = -90;
    else if (JXR > 90) JXR = 90;
    JYR = map(controller.rightJoyY(), 23, 234, -90, 90);
    if(JYR < -90) JYR = -90;
    else if (JYR > 90) JYR = 90;

    L = controller.triggerL();
    if(L >= 200) L = 90;
    else L = map(L, 24, 190, 0, 90);
    if (L < 0) L = 0;
    else if (L > 90) L = 90;

    R = controller.triggerR();
    if(R >= 200) R = 90;
    else R = map(R, 31, 180, 0, 90);
    if (R < 0) R = 0;
    else if (R > 90) R = 90;
  } 

  angle = atan2(JYL, JXL) * 180 / PI;

  if (pow(JXL, 2) + pow(JYL, 2) < pow(50, 2)){
    structController.dir = 0;
  } else if(angle >= -30 and angle <= 30){
    structController.dir = 1;
  } else if (angle >= 30 and angle <= 60){
    structController.dir = 2;
  } else if (angle >= 60 and angle <= 120){
    structController.dir = 3;
  } else if (angle >= 120 and angle <= 150){
    structController.dir = 4;
  } else if (angle >= 150 or angle <= -150){
    structController.dir = 5;
  } else if (angle >= -150 and angle <= -120){
    structController.dir = 6;
  } else if (angle >= -120 and angle <= -60){
    structController.dir = 7;
  } else if (angle >= -60 and angle <= -30){
    structController.dir = 8;
  }

  // if (ZL == 1){
  //   structController.dir = 9;
  // } else if(ZR == 1){
  //   structController.dir = 10;
  // } else if (L == 1){
  //   structController.dir = 11;
  // } else if (R == 1){
  //   structController.dir = 12;
  // }

  PrintValues();

  esp_now_send(broadcastAddress, (uint8_t *)&structController, sizeof(structController));
}

void PrintValues(){
  Serial.print(" JXL: "); Serial.print(JXL);
  Serial.print(" JYL: "); Serial.print(JYL); 
  Serial.print(" JXR: "); Serial.print(JXR);
  Serial.print(" JYR: "); Serial.print(JYR); 

  Serial.print(" ZL: "); Serial.print(ZL);
  Serial.print(" ZR: "); Serial.print(ZR);

  Serial.print(" L: "); Serial.print(L);
  Serial.print(" R: "); Serial.print(R);

  Serial.print(" dUp: "); Serial.print(dUp); 
  Serial.print(" dDown: "); Serial.print(dDown); 
  Serial.print(" dLeft: "); Serial.print(dLeft); 
  Serial.print(" dRight: "); Serial.print(dRight);

  Serial.print(" a: "); Serial.print(a); 
  Serial.print(" b: "); Serial.print(b); 
  Serial.print(" x: "); Serial.print(x); 
  Serial.print(" y: "); Serial.print(y);

  Serial.print(" selectB: "); Serial.print(selectB);
  Serial.print(" home: "); Serial.print(home); 
  Serial.print(" start: "); Serial.println(start); 
}