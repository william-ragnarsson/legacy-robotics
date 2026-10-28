#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

int num = 0;

void setup() {
  Serial.begin(9600);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(4);
}

void loop() {
  tft.fillScreen(TFT_BLACK);
  tft.drawString(String(num), 81, 64, 8);
  num++;
  delay(100);
}
