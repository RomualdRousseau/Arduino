#ifndef PISO_H
#define PISO_H

#include "common.h"

#define PISO_ENBL 2
#define PISO_LOAD 3
#define PISO_MOSI 4

void piso_init() {
  pinMode(PISO_MOSI, INPUT);
  pinMode(PISO_LOAD, OUTPUT);
  pinMode(PISO_ENBL, OUTPUT);
  digitalWrite(PISO_LOAD, HIGH);
  digitalWrite(PISO_ENBL, HIGH);
}

int piso_read() {
  int val = 0;

  // Enable
  digitalWrite(CLK_PIN, HIGH);
  digitalWrite(PISO_ENBL, LOW);
 
  // Load
  digitalWrite(PISO_LOAD, LOW);
  delayMicroseconds(CLK_SPEED);
  digitalWrite(PISO_LOAD, HIGH);

  // Read on failing clock
  
  for(int i = 0; i < 8; i++) {
    digitalWrite(CLK_PIN, LOW);
    if (digitalRead(PISO_MOSI) == HIGH) {
      val = (val << 1) | 0x01;
    } else {
      val = (val << 1);
    }
    delayMicroseconds(CLK_SPEED);

    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(CLK_SPEED);
  }

  // Disable
  digitalWrite(PISO_ENBL, HIGH);
  
  return val;
}

#endif
