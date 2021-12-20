#define BUFFER_NUM  4
#define SAMPLE_NUM  16

volatile unsigned int time;
volatile unsigned int samples[BUFFER_NUM][SAMPLE_NUM];
volatile unsigned int prevBuffer = 0;
volatile unsigned int currBuffer = 0;
volatile unsigned int currSample = 0;
volatile unsigned char sampleReady = 0;

void setup() {
  Serial.begin(9600);
  pinMode(5, INPUT_PULLUP);
  attachInterrupt(0, monitor, FALLING);
  time = millis();
}

void loop() {
  if(prevBuffer != currBuffer) {
    for(int i = 0; i < SAMPLE_NUM; i++) {
      Serial.println(samples[prevBuffer][i]);
    }
    prevBuffer = (prevBuffer + 1) % BUFFER_NUM;
  }
}

void monitor() {
  samples[currBuffer][currSample] = millis() - time;
  time = millis();
  
  currSample = (currSample + 1) % SAMPLE_NUM;
  if(currSample == 0) {
    prevBuffer = currBuffer;
    currBuffer = (currBuffer + 1) % BUFFER_NUM;
  }
}
