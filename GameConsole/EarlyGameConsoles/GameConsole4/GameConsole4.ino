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

//--------------------
//SETUP
//--------------------
void setup(){
  Serial.begin(9600);
  
  TFTInit();

  pinMode(JSPS, INPUT);
  digitalWrite(JSPS, HIGH);
}

//--------------------
//LOOP
//--------------------
void loop(){
  //SS Joystick (INPUT)
  UpdateJS();

  //SS Screen
  if(activeScreen == "Tron"){    
    if(millis() > timeNow + period){
      timeNow = millis();
      
      Tron();
    }
  }

  else if(activeScreen == "MainMenu"){
    MainMenu();
  }
}

//--------------------
//UPDATE JOYSTICK POSITION
//--------------------
void UpdateJS(){  
  const int JSPX = A1;
  const int JSPY = A0;
  
  int jsX = analogRead(JSPX);
  int jsY = analogRead(JSPY);
  char jsS = digitalRead(JSPS);

  jsDir[2] = -jsS;
  
  if(jsX > 900){
    jsDir[0] = -1;
    jsDir[1] = 0;
  }

  else if(jsX < 123){
    jsDir[0] = 1;
    jsDir[1] = 0;
  }

  else if(jsY > 900){
    jsDir[0] = 0;
    jsDir[1] = 1;
  }

  else if(jsY < 123){
    jsDir[0] = 0;
    jsDir[1] = -1;
  }

  else if(jsNeutral){
    jsDir[0] = 0;
    jsDir[1] = 0;
  }
}

//--------------------
//SCREEN FUNCTIONS
//--------------------
void TFTInit() {  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(BLACK);

  tft.setRotation(3);
}

void BlankScreen() {
  tft.fillScreen(BLACK);
}

void DrawPoint(int x, int y, uint16_t color){
  tft.drawPixel(x, y, color);
}

void DrawRect(int x, int y, int width, int height, uint16_t  color){
  tft.drawRect(x, y, width, height, color);
}

void DrawFillRect(int x, int y, int width, int height, uint16_t color) {
  tft.fillRect(x, y, width, height, color);
}

void DrawTextBox(int x, int y, int width, int boxHeight, int textHeightPx, String text, uint16_t boxColor, uint16_t textColor){
  //Rect
  DrawFillRect(x, y, width, boxHeight, boxColor);

  int textHeight = textHeightPx / 7;
  int textWidth = ( 5 * textHeight * text.length() ) + ( (text.length() - 1) * textHeight );

  int widthOffset = (width - textWidth) / 2;

  int heightOffset = (boxHeight - textHeightPx) / 2;
  
  //Text
  tft.setCursor(widthOffset + x, heightOffset + y);
  
  tft.setTextColor(textColor);
  tft.setTextSize(textHeight);
  
  tft.println(text);
}

//--------------------
//TRON
//--------------------
int bikePos[2] = {0, 0};
int bikeDir[2] = {0, 0};
int lastBikeDir[2] = {0, 0};

void Tron(){
  UpdateBikeDir();
  UpdateBikePos();

  if(bikeDir[0] != 0 or bikeDir[1] != 0){
    UpdateMatrix();
  }
  
  DrawBike();

  lastBikeDir[0] = bikeDir[0];
  lastBikeDir[1] = bikeDir[1];
}

void UpdateBikeDir(){
  if(lastBikeDir[0] != -jsDir[0] ){
    bikeDir[0] = jsDir[0];
  }
  if(lastBikeDir[1] != -jsDir[1]){
    bikeDir[1] = jsDir[1];
  }
}

void UpdateBikePos(){
  bikePos[0] += bikeDir[0];
  bikePos[1] += bikeDir[1];
}

void DrawBike(){
  DrawRect(bikePos[0] * 2, bikePos[1] * 2, 2, 2, BLUE);
}

unsigned char PowerReturn(unsigned char num){
  unsigned char arr[8] = {128, 64, 32, 16, 8, 4, 2, 1};
  return arr[num];
}

void UpdateMatrix(){
  unsigned char modResultX = (bikePos[0] - (bikePos[0] % 8))/8;

  if ((matrix[bikePos[1]][modResultX] & (PowerReturn(bikePos[0] % 8))) > 0){
    Serial.println("COLLISION");
  }
  
  
  matrix[bikePos[1]][modResultX] |= PowerReturn(bikePos[0] % 8);
}

/*
void PrintEntireMatrix(){
  for(unsigned char i = 0; i < 20; i++){
    for(unsigned char j = 0; j < 10; j++){
      if(j != 9){
        Serial.print(matrix[i][j]);
        if(matrix[i][j] < 10){
          Serial.print("   ");
        }else if(matrix[i][j] < 99){
          Serial.print("  ");
        }else{
          Serial.print(" ");
        }
        
      }
      else{
        Serial.println(matrix[i][j]);
      }
    }

    if(i == 19){
      Serial.println("----------------------------");
    }
  }
}
*/
