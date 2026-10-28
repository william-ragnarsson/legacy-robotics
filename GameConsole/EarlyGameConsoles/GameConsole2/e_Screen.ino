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
