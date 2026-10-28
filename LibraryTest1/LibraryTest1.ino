#include "my_library.h"

int result;

void setup() {
  Serial.begin(9600);

  result = AddTwoInts(5, 8);
}
void loop() {
}