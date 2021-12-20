#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

const byte header = 0x0F;
int channels[16];
byte flag = 0x00;
const byte footer = 0x00;

byte get_bit(byte buf[], int off) {
  int p = off / 8;
  byte b = 1 << (off % 8);
  return buf[p] & b;
}

void set_bit(byte buf[], int off, byte val) {
  int p = off / 8;
  byte b = 1 << (off % 8);

  if (val == 0) {
    buf[p] &= ~b;
  } else {
    buf[p] |= b;
  }
}

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int b = analogRead(A0);
  int v = map(b, 0, 1023, 1000, 2000);
  
  channels[0]= v;
  channels[1]= 0;
  channels[2]= 0;
  channels[3]= 0;
  channels[4]= 0;
  channels[5]= 0;
  channels[6]= 0;
  channels[7]= 0;
  channels[8]= 0;
  channels[9]= 0;
  channels[10]= 0;
  channels[11]= 0;
  channels[12]= 0;
  channels[12]= 0;
  channels[14]= 0;
  channels[15]= 0;

  byte tmp[22];
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 11; j++) {
      byte val = get_bit((byte*) &channels[i], j);
      set_bit(tmp, i * 11 + j, val);
    }
  }

  radio.write(&header, 1);
  radio.write(tmp, 22);
  radio.write(&flag, 1);
  radio.write(&footer, 1);
  
  delay(10);
}
