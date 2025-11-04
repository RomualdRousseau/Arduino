#ifndef CLOCK_H
#define CLOCK_H

#include "common.h"

void clock_init() {
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, LOW);
}

#endif
