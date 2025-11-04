#include <RF24.h>

const byte address[6] = "00001";
const byte header = 0x0F;

typedef struct
{
  byte header;
  byte data[22];
  byte flag;
  byte footer;
} packet_t;

RF24 radio(10, 8); // CE, CSN
int channels[16];

void setup() {

  for (int i = 0; i < 16; i++) {
    channels[i] = 1500;
  }
  
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  const int b = analogRead(A0);
  const int v = map(b, 0, 1023, 1000, 2000);
  channels[0]= v;

  packet_t packet;
  encode_packet(channels, &packet);
  radio.write(&packet, sizeof(packet));
  delay(10);
}

void encode_packet(const int* channels, packet_t* packet) {
  packet->header = header;
  packet->flag = 0;

  byte sum = 0;
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 11; j++) {
      const byte val = get_bit((byte*) &channels[i], j);
      set_bit(packet->data, i * 11 + j, val);
      sum += val;
    }
  }

  packet->footer = packet->header + packet->flag + sum;
}

byte get_bit(const byte buf[], const int off) {
  int p = off / 8;
  byte b = 1 << (off % 8);
  return buf[p] & b;
}

void set_bit(byte buf[], const int off, const byte val) {
  int p = off / 8;
  byte b = 1 << (off % 8);

  if (val == 0) {
    buf[p] &= ~b;
  } else {
    buf[p] |= b;
  }
}
