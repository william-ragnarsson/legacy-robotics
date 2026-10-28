// ESP NOW import
#include <ESP8266WiFi.h>
#include <espnow.h>
// ESP NOW nintendo
#include <Wire.h>
#include <NintendoExtensionCtrl.h>

#define SCL1 5
#define SDA1 4

#define SCL2 0
#define SDA2 2

// MECANUM
uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0xCB, 0xA6, 0xB0};

void ESPInit()
{
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0)
  {
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

ClassicController controller;

/* {
  0: S
  1: R
  2: FR
  3: F
  4: FL
  5: L
  6: BL
  7: B
  8: BR
  9: TR
  10: TL
  11: SR
  12: SL
} */

struct struct_message
{
  String dir;
};
struct_message structController;

bool dUp, dDown, dLeft, dRight, a, b, x, y, ZL, ZR, selectB, home, start;
double JXL, JYL, JXR, JYR, L, R;

double angle;

void setup()
{
  Serial.begin(115200);

  ESPInit();
}

void loop()
{

  Wire.begin(SDA1, SCL1);
  if (!controller.connect())
  {
    Serial.println("#1 NOT CONNECTED!");
  }
  else
  {
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
    if (JXL < -90)
      JXL = -90;
    else if (JXL > 90)
      JXL = 90;
    JYL = map(controller.leftJoyY(), 30, 236, -90, 90);
    if (JYL < -90)
      JYL = -90;
    else if (JYL > 90)
      JYL = 90;

    JXR = map(controller.rightJoyX(), 17, 220, -90, 90);
    if (JXR < -90)
      JXR = -90;
    else if (JXR > 90)
      JXR = 90;
    JYR = map(controller.rightJoyY(), 23, 234, -90, 90);
    if (JYR < -90)
      JYR = -90;
    else if (JYR > 90)
      JYR = 90;

    L = controller.triggerL();
    if (L >= 200)
      L = 90;
    else
      L = map(L, 24, 190, 0, 90);
    if (L < 0)
      L = 0;
    else if (L > 90)
      L = 90;

    R = controller.triggerR();
    if (R >= 200)
      R = 90;
    else
      R = map(R, 31, 180, 0, 90);
    if (R < 0)
      R = 0;
    else if (R > 90)
      R = 90;
  }

  angle = atan2(JYL, JXL) * 180 / PI;

  if (R > 50 and L > 50){
    structController.dir = "S";
  }
  else if (R > 50){
    structController.dir = "TR";
  }
  else if (L > 50){
    structController.dir = "TL";
  }
  else if (ZL and ZR){
    structController.dir = "S";
  }
  else if (ZL) {
    structController.dir = "SL";
  }
  else if (ZR) {
    structController.dir = "SR";
  }
  else if (pow(JXL, 2) + pow(JYL, 2) < pow(50, 2))
  {
    structController.dir = "S";
  }
  else if (angle >= -30 and angle <= 30)
  {
    structController.dir = "R";
  }
  else if (angle >= 30 and angle <= 60)
  {
    structController.dir = "FR";
  }
  else if (angle >= 60 and angle <= 120)
  {
    structController.dir = "F";
  }
  else if (angle >= 120 and angle <= 150)
  {
    structController.dir = "FL";
  }
  else if (angle >= 150 or angle <= -150)
  {
    structController.dir = "L";
  }
  else if (angle >= -150 and angle <= -120)
  {
    structController.dir = "BL";
  }
  else if (angle >= -120 and angle <= -60)
  {
    structController.dir = "B";
  }
  else if (angle >= -60 and angle <= -30)
  {
    structController.dir = "BR";
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

void PrintValues()
{
  Serial.print(structController.dir);
  Serial.print(" JXL: ");
  Serial.print(JXL);
  Serial.print(" JYL: ");
  Serial.print(JYL);
  Serial.print(" JXR: ");
  Serial.print(JXR);
  Serial.print(" JYR: ");
  Serial.print(JYR);

  Serial.print(" ZL: ");
  Serial.print(ZL);
  Serial.print(" ZR: ");
  Serial.print(ZR);

  Serial.print(" L: ");
  Serial.print(L);
  Serial.print(" R: ");
  Serial.print(R);

  Serial.print(" dUp: ");
  Serial.print(dUp);
  Serial.print(" dDown: ");
  Serial.print(dDown);
  Serial.print(" dLeft: ");
  Serial.print(dLeft);
  Serial.print(" dRight: ");
  Serial.print(dRight);

  Serial.print(" a: ");
  Serial.print(a);
  Serial.print(" b: ");
  Serial.print(b);
  Serial.print(" x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);

  Serial.print(" selectB: ");
  Serial.print(selectB);
  Serial.print(" home: ");
  Serial.print(home);
  Serial.print(" start: ");
  Serial.println(start);
}