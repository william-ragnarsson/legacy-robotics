#include <TFT.h>  
#include <SPI.h>

#define cs   10
#define dc   9
#define rst  8

TFT TFTscreen = TFT(cs, dc, rst);

int num = 0;
char sensorPrintout[4];

void setup() {
  TFTscreen.begin();
  
  TFTscreen.background(0, 0, 0);
}

void loop() {

  TFTscreen.stroke(0, 0, 255);
  TFTscreen.rect(50,34,60,60);

  TFTscreen.stroke(0, 0, 0);
  TFTscreen.rect(50,34,60,60);

  TFTscreen.text(sensorPrintout, 0, 0);

  //num = num + 1;
}
