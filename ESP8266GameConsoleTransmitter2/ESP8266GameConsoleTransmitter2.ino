#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t broadcastAddress[] = { 0xB8, 0xD6, 0x1A, 0x44, 0x3B, 0x4C };

struct struct_message {
  int x;
  int y;
  int s;
};
struct_message js;

int D1 = 5;
int D2 = 4;
int D3 = 0;

void setup() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);


  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
}

void loop() {
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  delay(5);
  js.x = analogRead(A0);
  if(js.x > 900) js.x = 1;
  else if(js.x < 700) js.x = -1;
  else js.x = 0;

  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  delay(5);
  js.y = analogRead(A0);
  if(js.y > 900) js.y = 1;
  else if(js.y < 700) js.y = -1;
  else js.y = 0;


  js.s = digitalRead(D3);
  if(js.s == 1) js.s = 0;
  else js.s = 1;

  esp_now_send(broadcastAddress, (uint8_t *)&js, sizeof(js));
}