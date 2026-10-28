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
