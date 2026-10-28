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
