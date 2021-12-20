#define FLAGS_CS  2
#define FLAGS_CK  3
#define FLAGS_RW  4

byte addr;
byte flags;
byte data;

void setup() {

  // Configure pin inputs and outputs

  DDRA = 0;
  DDRB = 0;
  DDRC = 0;
  DDRD = 0;

  // Configure serial output

  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Logic Analyzer v1.0");
  Serial.println();
}

void loop() {
  while(PIND & ((1 << FLAGS_CS) | (1 << FLAGS_CK)))
  {
    ; // wait for port to be ready
  }

  addr = PINC;
  flags = PIND;

  while(!(PIND & (1 << FLAGS_CK)))
  {
    ; // wait for new data
  }

  data = PINB;

  char buf[20];
  sprintf(buf, "%02x %02x %c %c %c",
    addr,
    data,
    flags & (1 << FLAGS_RW) ? 'r' : 'W',
    flags & (1 << FLAGS_CS) ? 'd' : 'E',
    flags & (1 << FLAGS_CK) ? '1' : '0');
  Serial.println(buf);
}
