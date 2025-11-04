#ifndef SIPO_H
#define SIPO_H

#include "common.h"

#define SIPO_SRST 5
#define SIPO_LTCH 6
#define SIPO_MISO 7


void sipo_init() {
  pinMode(SIPO_SRST, OUTPUT);
  pinMode(SIPO_MISO, OUTPUT);
  pinMode(SIPO_LTCH, OUTPUT);
  digitalWrite(SIPO_SRST, LOW);
  digitalWrite(SIPO_MISO, LOW);
  digitalWrite(SIPO_LTCH, LOW);
}

void sipo_write(const int val) {

  // Enable
  digitalWrite(CLK_PIN, LOW);
  
  // Load
  digitalWrite(SIPO_SRST, LOW);
  delayMicroseconds(CLK_SPEED);
  digitalWrite(SIPO_SRST, HIGH);

  // Write Value
  for(int i = 0; i < 8; i++) {
    digitalWrite(SIPO_MISO, (val >> (7 - i)) & 0x01);
    
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(CLK_SPEED);
    
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(CLK_SPEED);
  }

  // Latch
  digitalWrite(SIPO_LTCH, HIGH);
  delayMicroseconds(CLK_SPEED);
  digitalWrite(SIPO_LTCH, LOW);
}

#endif
