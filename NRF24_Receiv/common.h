#ifndef COMMON_H
#define COMMON_H

#define MESSAGE "NRF24L01 Receiver v1.0"

const int radio_ce_pin = A4;
const int radio_csn_pin = A5;

const int motor1_powr_pin = 3;
const int motor1_dir1_pin = 2;
const int motor1_dir2_pin = 4;

const int motor2_powr_pin = 6;
const int motor2_dir1_pin = 5;
const int motor2_dir2_pin = 7;

const int servo1_pin = 9;
const int servo2_pin = 10;

const byte address[6] = "00001";
const byte header = 0x0F;

#endif
