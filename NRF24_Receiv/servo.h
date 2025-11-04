#include "common.h"

#include <Servo.h>

Servo servo1;
Servo servo2;

void servo_init() {
  servo1.attach(servo1_pin);
  servo2.attach(servo2_pin);
}

void servo_update(const int value) {
  servo1.write(value);
  servo2.write(value);
}
