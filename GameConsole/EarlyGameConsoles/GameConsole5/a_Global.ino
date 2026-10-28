//------------------------
/*//SCREEN 160x128 (159x127)
//------------------------

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
extern const uint16_t WHITE = 0xFFFF;
extern const uint16_t BLUE = 0x001F;
extern const uint16_t RED = 0xF800;
extern const uint16_t GREEN = 0x07E0;

//---------------
//INPUT
//---------------

//Button pinout
const byte btnPin = 3;
byte btnState;

//Joystick pinout
const byte JSpin = 2;
bool jsNeutral = false;
byte jsDir[3] = {0, 0, 0};

//---------------
//"LOGISTICS"
//---------------

byte screenMatrix[64][10] = {0};
