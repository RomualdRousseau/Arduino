#include "common.h"

void motor_init() {

  // Motor 1
  pinMode(motor1_powr_pin, OUTPUT);
  pinMode(motor1_dir1_pin, OUTPUT);
  pinMode(motor1_dir2_pin, OUTPUT);
  
  analogWrite(motor1_powr_pin, 0);
  digitalWrite(motor1_dir1_pin, LOW);
  digitalWrite(motor1_dir2_pin, LOW);

  // Motor 2

  pinMode(motor2_powr_pin, OUTPUT);
  pinMode(motor2_dir1_pin, OUTPUT);
  pinMode(motor2_dir2_pin, OUTPUT);

  analogWrite(motor2_powr_pin, 0);
  digitalWrite(motor2_dir1_pin, LOW);
  digitalWrite(motor2_dir2_pin, LOW);
}

void motor1_update(const int value) {
  if (value <= -32) {
    analogWrite(motor1_powr_pin, map(-value, 0, 127, 0, 255));
    digitalWrite(motor1_dir1_pin, HIGH);
    digitalWrite(motor1_dir2_pin, LOW);
  }
  else if (value >=32) {
    analogWrite(motor1_powr_pin, map(value, 0, 128, 0, 255));
    digitalWrite(motor1_dir1_pin, LOW);
    digitalWrite(motor1_dir2_pin, HIGH);
  }
  else {
    analogWrite(motor1_powr_pin, 0);
    digitalWrite(motor1_dir1_pin, LOW);
    digitalWrite(motor1_dir2_pin, LOW);
  }
}

void motor2_update(const int value) {
  if (value <= -32) {
    analogWrite(motor2_powr_pin, map(-value, 0, 127, 0, 255));
    digitalWrite(motor2_dir1_pin, HIGH);
    digitalWrite(motor2_dir2_pin, LOW);
  }
  else if (value >=32) {
    analogWrite(motor2_powr_pin, map(value, 0, 128, 0, 255));
    digitalWrite(motor2_dir1_pin, LOW);
    digitalWrite(motor2_dir2_pin, HIGH);
  }
  else {
    analogWrite(motor2_powr_pin, 0);
    digitalWrite(motor2_dir1_pin, LOW);
    digitalWrite(motor2_dir2_pin, LOW);
  }
}
