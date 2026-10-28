//Joystick Pinout
const int JSPX = A1;
const int JSPY = A0;

//Joystick x, y Values
int jsX;
int jsY;

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
