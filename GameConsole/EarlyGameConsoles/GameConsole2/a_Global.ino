//--------------------
//SCREEN
//--------------------

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

//Joystick pinout
const int JSPS = 2;

int jsDir[3] = {0, 0, 0};

bool jsNeutral = false;

//--------------------
//Other Variables
//--------------------

//Timer
unsigned char period = 60;
unsigned long timeNow = 0;

//ActiveGame
String activeScreen = "Tron";

//Pixel
unsigned char matrix[64][10] = {0};

//--------------------
//FUNCTIONS
//--------------------

void Tron();
