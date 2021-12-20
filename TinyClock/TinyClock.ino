#define CLK     0
#define MAN     1
#define SET     2
#define HLT     3
#define OUT     4

void setup() {
  pinMode(CLK, INPUT);
  pinMode(MAN, INPUT);
  pinMode(SET, INPUT);
  pinMode(HLT, INPUT);
  pinMode(OUT, OUTPUT);
}

void loop() {
  int clk = digitalRead(CLK);
  int man = digitalRead(MAN);
  int set = digitalRead(SET);
  int hlt = digitalRead(HLT);
  digitalWrite(OUT, !hlt && (set && clk || !set && man));
}
