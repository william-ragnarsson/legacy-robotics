#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9




Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void TFTInit(){
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(BLACK);

  tft.setRotation(3);
}

void DrawFillRect(int x, int y, int width, int height){
  tft.fillRect(x, y, width, height, WHITE);
}

void BlankScreen(){
  tft.fillScreen(BLACK);
}
