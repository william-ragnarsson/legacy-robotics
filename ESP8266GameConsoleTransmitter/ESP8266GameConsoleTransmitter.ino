#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t broadcastAddress[] = { 0xB8, 0xD6, 0x1A, 0x44, 0x3B, 0x4C };

struct struct_message {
  int upPressed;
  int downPressed;
  int rightPressed;
  int leftPressed;

  // int upClicked;
  // int downClicked;
  // int rightClicked;
  // int leftClicked;
};

// int upPressed;
// int downPressed;
// int rightPressed;
// int leftPressed;

// bool isUpClicked = false;
// bool isDownClicked = false;
// bool isRightClicked = false;
// bool isLeftClicked = false;

struct_message js;

//My shit
int upPin = 5;
int downPin = 4;
int rightPin = 12;
int leftPin = 14;

void setup() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  //My shit
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(leftPin, INPUT);
}

void loop() {
  js.upPressed = digitalRead(upPin);
  js.downPressed = digitalRead(downPin);
  js.rightPressed = digitalRead(rightPin);
  js.leftPressed = digitalRead(leftPin);

  // //UpClicked
  // if(js.upPressed == 1 and isUpClicked == false){
  //   js.upClicked = 1;
  //   isUpClicked = true;
  // } else js.upClicked = 0;
  // if(js.upPressed == 0) isUpClicked = false;

  // //DownClicked
  // if(js.downPressed == 1 and isDownClicked == false){
  //   js.downClicked = 1;
  //   isDownClicked = true;
  // } else js.downClicked = 0;
  // if(js.downPressed == 0) isDownClicked = false;

  // //RightClicked
  // if(js.rightPressed == 1 and isRightClicked == false){
  //   js.rightClicked = 1;
  //   isRightClicked = true;
  // } else js.rightClicked = 0;
  // if(js.rightPressed == 0) isRightClicked = false;
  
  // //LeftClicked
  // if(js.leftPressed == 1 and isLeftClicked == false){
  //   js.leftClicked = 1;
  //   isLeftClicked = true;
  // } else js.leftClicked = 0;
  // if(js.leftPressed == 0) isLeftClicked = false;

  //UpClicked

  esp_now_send(broadcastAddress, (uint8_t *)&js, sizeof(js));
}