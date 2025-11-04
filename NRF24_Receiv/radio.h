#include "common.h"
#include "util.h"

#include <RF24.h>

typedef struct
{
  byte header;
  byte data[22];
  byte flag;
  byte footer;
} packet_t;

RF24 radio(radio_ce_pin, radio_csn_pin);

void decode_packet(const packet_t* packet, int* channels) {
  byte sum = packet->header + packet-> flag;
  byte off = 0;
  for (int i = 0; i < 16; i++) {
    channels[i] = 0;
    for (int j = 0; j < 11; j++) {
      byte val = array_get_bit(packet->data, off);
      channels[i] |= (val << j);
      sum += val;
      off += 1;
    }
  }
  if (packet->footer != sum) {
    Serial.println("error");
  }
}

void radio_init() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void radio_update(int* channels) {
  if (radio.available()) {
    packet_t packet;
    radio.read(&packet, sizeof(packet));
    if(packet.header == 0x0F) {
      decode_packet(&packet, channels);
      Serial.print("Received one packet: ");
      Serial.print(channels[0]);
      Serial.print(" ");
      Serial.println(channels[1]);
    }
  }
}
