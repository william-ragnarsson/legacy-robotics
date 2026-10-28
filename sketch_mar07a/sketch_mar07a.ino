#include <Mouse.h>

void setup() {
  Mouse.begin();
}

void loop() {
  Mouse.move(10, 10, 2)
}
