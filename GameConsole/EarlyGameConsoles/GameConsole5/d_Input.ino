/*
void UpdateBtnState(){  
  btnState = digitalRead(btnPin);
}

void UpdateJS(){  
  const int JSPX = A5;
  const int JSPY = A4;
  
  int jsX = analogRead(JSPX);
  int jsY = analogRead(JSPY);
  byte jsS = digitalRead(JSPS);

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
}*/
