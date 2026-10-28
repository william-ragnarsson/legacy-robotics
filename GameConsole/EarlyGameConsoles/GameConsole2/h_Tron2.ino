/*void Tron(){
  for(unsigned char i = 0; i < 2; i++){
    UpdateBikeDir(bikeArr[i]);
    UpdateBikePos(bikeArr[i]);
  
    UpdateMatrix(bikeArr[i]);
    
    DrawBike(bikeArr[i]);
  
    UpdateLastBikeDir(bikeArr[i]);
  }
  
}

void UpdateBikeDir(Bike& bike){
  if(bike.lastBikeDir[0] != -jsDir[0] ){
    bike.bikeDir[0] = jsDir[0];
  }
  if(bike.lastBikeDir[1] != -jsDir[1]){
    bike.bikeDir[1] = jsDir[1];
  }
}

void UpdateLastBikeDir(Bike& bike){
  bike.lastBikeDir[0] = bike.bikeDir[0];
  bike.lastBikeDir[1] = bike.bikeDir[1];
}

void UpdateBikePos(Bike& bike){
  Serial.println(bike2.bikePos[0]);
  bike.bikePos[0] += bike.bikeDir[0];
  bike.bikePos[1] += bike.bikeDir[1];
}

void DrawBike(Bike& bike){
  DrawRect(bike.bikePos[0] * 2, bike.bikePos[1] * 2, 2, 2, BLUE);
}

unsigned char PowerReturn(unsigned char num){
  unsigned char arr[8] = {128, 64, 32, 16, 8, 4, 2, 1};
  return arr[num];
}

void UpdateMatrix(Bike& bike){
  if(bike.bikeDir[0] != 0 or bike.bikeDir[1] != 0){
    unsigned char modResultX = (bike.bikePos[0] - (bike.bikePos[0] % 8))/8;
  
    if ((matrix[bike.bikePos[1]][modResultX] & (PowerReturn(bike.bikePos[0] % 8))) > 0){
      Serial.println("COLLISION");
    }
    
    
    matrix[bike.bikePos[1]][modResultX] |= PowerReturn(bike.bikePos[0] % 8);
  }
  else{
    bike2.bikePos[0] = 50;
  }
}/*
