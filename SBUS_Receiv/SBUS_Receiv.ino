#include <PPMDevice.h>

int channels[16];
byte flag;

void setup() {
  for (int i = 0; i < 16; i++) {
    channels[i] = 1600;
  }
  
  Serial.begin(100000, SERIAL_8E2);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  PPMDevice.begin();

  for (int i = 0; i < PPM_MAX_CHANNELS; i++) {
    PPMDevice.analogWrite(i, channels[i]);
  }
}

void loop() {
  if (Serial.available()) {
    byte header;
    Serial.readBytes(&header, 1);
    if(header == 0x0F && read_one_packet()) {
      for (int i = 0; i < PPM_MAX_CHANNELS; i++) {
        PPMDevice.analogWrite(i, map(channels[i], 172, 1811, 0, 1023));
      }
    }
  }
}

bool read_one_packet() {
  byte _payload[24];
  
  if (Serial.readBytes(_payload, 24) != 24) {
    return false;
  }
  
  if (_payload[23] != 0x00) {
    return false;
  }

  channels[0]  = (uint16_t) ((_payload[0]    |_payload[1] <<8)                   & 0x07FF);
  channels[1]  = (uint16_t) ((_payload[1]>>3 |_payload[2] <<5)                   & 0x07FF);
  channels[2]  = (uint16_t) ((_payload[2]>>6 |_payload[3] <<2 |_payload[4]<<10)  & 0x07FF);
  channels[3]  = (uint16_t) ((_payload[4]>>1 |_payload[5] <<7)                   & 0x07FF);
  channels[4]  = (uint16_t) ((_payload[5]>>4 |_payload[6] <<4)                   & 0x07FF);
  channels[5]  = (uint16_t) ((_payload[6]>>7 |_payload[7] <<1 |_payload[8]<<9)   & 0x07FF);
  channels[6]  = (uint16_t) ((_payload[8]>>2 |_payload[9] <<6)                   & 0x07FF);
  channels[7]  = (uint16_t) ((_payload[9]>>5 |_payload[10]<<3)                   & 0x07FF);
  channels[8]  = (uint16_t) ((_payload[11]   |_payload[12]<<8)                   & 0x07FF);
  channels[9]  = (uint16_t) ((_payload[12]>>3|_payload[13]<<5)                   & 0x07FF);
  channels[10] = (uint16_t) ((_payload[13]>>6|_payload[14]<<2 |_payload[15]<<10) & 0x07FF);
  channels[11] = (uint16_t) ((_payload[15]>>1|_payload[16]<<7)                   & 0x07FF);
  channels[12] = (uint16_t) ((_payload[16]>>4|_payload[17]<<4)                   & 0x07FF);
  channels[13] = (uint16_t) ((_payload[17]>>7|_payload[18]<<1 |_payload[19]<<9)  & 0x07FF);
  channels[14] = (uint16_t) ((_payload[19]>>2|_payload[20]<<6)                   & 0x07FF);
  channels[15] = (uint16_t) ((_payload[20]>>5|_payload[21]<<3)                   & 0x07FF);

  flag = _payload[22];

  return true;
}
