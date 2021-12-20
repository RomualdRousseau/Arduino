#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
Servo myServo;
int channels[16];

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
  Serial.begin(115200);

  for (int i = 0; i < 16; i++) {
    channels[i] = 1500;
  }
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  myServo.attach(2);
}

void loop() {
  update_commands();
  
  if (radio.available()) {
    byte header;
    radio.read(&header, 1);
    if(header == 0x0F) {
      read_one_packet();
    }
  }

  delay(10);
}

void read_one_packet() {
  byte tmp[22];
  radio.read(tmp, 22);
      
  byte flag;
  radio.read(&flag, 1);
  
  byte footer;
  radio.read(&footer, 1);

  if (footer != 0x00) {
    Serial.println("error");
    return;
  }

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 11; j++) {
      byte val = get_bit(tmp, i * 11 + j);
      set_bit((byte*) channels, i * 16 + j, val);
    }
  }
}

void update_commands() {
  myServo.writeMicroseconds(channels[0]);
  //Serial.println(channels[0]);
}
