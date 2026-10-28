float snakeSpeed = 1.0;
int snakeFoodCount = 1;
String snakePortal = "on";
String snakeInverted = "off";

void Snake(){
  if(ncClicked){
    gameConsoleIndex = GCI("SettingsMenu");
    SettingsInit();
    return;
  }

  // Snake game
  tft.fillScreen(TFT_GREEN);
}