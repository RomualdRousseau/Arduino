#define DMA       (1<<2)
#define CLOCK    (1<<3)
#define NMI         (1<<4)
#define SELECT   (1<<5)
#define RW          (1<<6)

void setup() {
  Serial.begin(115200);

  initDMA();

  delay(1000);
}

void loop() {

  lockDMA();

  Serial.println("Took control of the DMA");
  delay(1000);

  PORTA = 0x04;
  PORTC = 0x00;
  waitFewClockPulse();
  int value = PINB;

  unlockDMA();

  Serial.println(value);
  Serial.println("Released control of the DMA");
  delay(1000);

  signalNMI();

  Serial.println("Signaled NMI");
  delay(1000);
}

void initDMA() {
  waitClock();
  PORTD |= DMA | NMI;
  DDRD |= DMA | NMI;
}

void lockDMA() {
  
  waitCPU();

  // Take DMA control

  PORTD = (PORTD & ~DMA) | RW;
  DDRD |= RW;

  // Take address bus control

  DDRA = B11111111;
  DDRC = B11111111;

  // Take data bus control

  DDRB = B00000000;
}

void unlockDMA() {
  
  waitClock();

  // Release DMA control

  PORTD |= DMA;
  DDRD &= ~RW;
  
  // Release data bus control

  DDRB = B00000000;

  // Release address bus control

  DDRC = B00000000;
  DDRA = B00000000;
}

void signalNMI() {
  PORTD &= ~NMI;
  waitFewClockPulse();
  PORTD |= NMI;
}

inline void waitCPU() {
  while ((PIND & (CLOCK | RW)) != RW);
}

inline void waitClock() {
  while ((PIND & (CLOCK)) != 0);
}

inline void waitFewClockPulse() {
  delayMicroseconds(2);
}
