String pongBallBounce = "{";
float pongBallSpeed = 1.0;
float pongBoardSpeed = 1.0;
String pongInverted = "off";

void Pong(){
  if(ncClicked){
    gameConsoleIndex = GCI("SettingsMenu");
    SettingsInit();
    return;
  }

  // Pong game
  tft.fillScreen(TFT_BLUE);
}