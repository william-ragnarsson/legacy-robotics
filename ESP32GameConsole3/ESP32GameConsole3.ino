#include "my_library.h"
#include <TFT_eSPI.h>
#include <SPI.h>

//Definitions
#define PI 3.14159265358979323846

TFT_eSPI tft = TFT_eSPI();

//VARIABLES
StructJoystick js;

const unsigned long frameRateInterval = 20;
unsigned long previousFrameTime = 0;

bool gameOver = false;

//ACHTUNG DIE KURVE

StructKurve kurve1;

void AchtungDieKurve(){

  //Update Angle and Position of Kurve
  UpdateKurveAngle(&kurve1, &js);
  UpdateKurvePos(&kurve1);

  //Update some values
  UpdateKurveIntVals(&kurve1);

  //Check for collision
  if(IsKurveCollision(&kurve1)){
    // GameOver();
    Serial.println("GAME OVER");
  }
  
  if(LeaveKurveGap(&kurve1) == false){
    //Update matrix
    UpdateKurveMatrix(&kurve1);

    //Draw Kurve pixel
    tft.drawPixel(kurve1.intX, kurve1.intY, TFT_WHITE);
  }

  //Update some values
  UpdateKurvePrevPos(&kurve1);
}

void MainMenu(){

}

void setup(){
  //Init Serial monitor
  Serial.begin(9600);

  //Init ESP NOW communication
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(OnDataRecv);

  //Init tft LCD screen
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void loop(){
  int currentTime = millis();

  if(currentTime - previousFrameTime >= frameRateInterval){
    previousFrameTime = currentTime;
  }

  

  if(js.up == 1){
    tft.drawPixel(10, 10, TFT_WHITE);
    Serial.println("ISWEFGJPO:gHPSIWEgHE OgHEOIUH OIH OIgHEIUWSgWE:LIUgWE");
  }
  else{
    tft.drawPixel(10, 10, TFT_BLACK);
  }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&js, incomingData, sizeof(js));
}