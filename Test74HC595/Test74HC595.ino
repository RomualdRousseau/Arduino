#include "clock.h"
#include "piso.h"
#include "sipo.h"

void setup() {
  Serial.begin(115200);
  clock_init();
  piso_init();
  sipo_init();
}

void loop() {
  const int val = piso_read();
  sipo_write(~val);
  Serial.println(val, HEX);
}
