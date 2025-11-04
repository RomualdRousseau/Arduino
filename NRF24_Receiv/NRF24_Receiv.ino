#include "radio.h"
#include "motor.h"
#include "servo.h"

int channels[16];

void setup() {
  Serial.begin(115200);
  Serial.println(MESSAGE);

  for (int i = 0; i < 16; i++) {
    channels[i] = 1500;
  }

  motor_init();
  radio_init();
  servo_init();

  motor1_update(map(1700, 1000, 2000, -127, 128));
  motor2_update(map(1700, 1000, 2000, -127, 128));
  delay(500);
  motor1_update(map(1500, 1000, 2000, -127, 128));
  motor2_update(map(1500, 1000, 2000, -127, 128));
  delay(50);
  motor1_update(map(1300, 1000, 2000, -127, 128));
  motor2_update(map(1300, 1000, 2000, -127, 128));
  delay(500);
  motor1_update(map(1500, 1000, 2000, -127, 128));
  motor2_update(map(1500, 1000, 2000, -127, 128));
  delay(50);  
}

void loop() {
//  motor1_update(map(channels[0], 1000, 2000, -127, 128));
//  motor2_update(map(channels[1], 1000, 2000, -127, 128));
//  radio_update(channels);
//  delay(10);

  int pos = 0;
  for (pos = 0; pos <= 180; pos += 1) {
    servo_update(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    servo_update(pos);
    delay(15);
  }
}
