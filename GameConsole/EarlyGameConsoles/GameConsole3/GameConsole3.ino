//-----------------------------------------------
//-----------------------------------------------
//                  VARIABLES
//-----------------------------------------------
//-----------------------------------------------

/*--------------------
SCREEN
--------------------*/

//Include TFT screen libraries
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

//Define TFT screen pins
#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//Define color codes
extern const uint16_t BLACK = 0x0000;
extern const uint16_t WHITE = 0xffff;

/*---------------
INPUT
---------------*/

//Joystick pinout
const int JSPX = A1;
const int JSPY = A0;

//Joystick x, y values
int jsX;
int jsY;

/*---------------
Tron game
---------------*/

bikePos[2] = {0, 0};
bikeDir[2] = {0, 0};

//-----------------------------------------------
//-----------------------------------------------
//                  FUNCTIONS
//-----------------------------------------------
//-----------------------------------------------

/*---------------
INPUT
---------------*/
String JS(){
  jsX = analogRead(JSPX);
  jsY = analogRead(JSPY);
  
  if(jsX > 1013){
    return "LEFT";
  }

  else if(jsX < 10){
    return "RIGHT";
  }

  else if(jsY > 1013){
    return "DOWN";
  }

  else if(jsY < 10){
    return "UP";
  }

  return "MIDDLE";
}

void setup() {
  /*--------------------
  SCREEN
  --------------------*/
  //Screen setup
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(BLACK);
  tft.setRotation(3);

  
}

void loop() {
  
}
