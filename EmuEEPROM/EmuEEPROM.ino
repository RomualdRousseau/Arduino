PROGMEM const byte bank[32*1024] = {
  0xa9, 0xff, 0x8d, 0x02, 0x60, 0xa9, 0x55, 0x8d, 0x00, 0x60, 0xa9, 0xaa, 0x8d, 0x00, 0x60, 0x4c,
  0x05, 0x80
  };

void setup() {
  
  // Setup clock interrupt
  EECR |= _BV(INT0);
  MCUCR = MCUCR & ~(_BV(ISC00) | _BV(ISC01)) | _BV(ISC00);

  // ADDR Lines
  DDRA = B00000000;
  DDRC = B00000000;

  // DATA Lines
  DDRB = B00000000;

  // CTRL Lines
  DDRD = B00000000;

  sei();
  
  bank2[0xFFFC] = 0x00;
  bank2[0xFFFD] = 0x80;
}

void loop() {
}

ISR(INT0_vect) {
  if ((PIND & _BV(3)) > 0) {
    DDRB = B11111111;
    int addr = (PINA << 8 | PINC) & (~0x8000);
    PORTB = bank1[addr];
  } else {
    DDRB = B00000000;
  }
}
