#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#include <Fonts/FreeSerif18pt7b.h>

int Variable1;

void setup() {
  tft.initR(INITR_BLACKTAB);

  tft.fillScreen(ST7735_BLACK);
}

void loop() {
  tft.fillRect(0, 60, 60, 30, ST7735_BLACK);

  delay(500);
  
  tft.fillRect(0, 60, 60, 30, ST7735_WHITE);

  delay(500);
}
