#include<SPI.h>

volatile boolean spi_received;
volatile byte spi_data;

char buffer[32];

void setup() {
  Serial.begin(115200);
  Serial.println("Receiving ...");
  
  spi_begin();
}

void loop() {
  int data = spi_transfer_byte(0xAA);
  if (data >= 0) {
    sprintf(buffer, "Received: %d", data);
    Serial.println(buffer);
  }
}

void spi_begin() {
  pinMode(SS, INPUT_PULLUP);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SCK, INPUT);
  SPCR |= _BV(SPE);
  spi_received = false;
  SPI.attachInterrupt();
}

int spi_transfer_byte(byte return_data) {
  if(spi_received) {
    SPDR = spi_data;
    spi_received = false;
    return spi_data;
  } else {
    return -1;
  }
}

void spi_write_byte(byte data) {
  SPDR = data;
}

ISR (SPI_STC_vect) {
  spi_data = SPDR;
  spi_received = true;                     
}
