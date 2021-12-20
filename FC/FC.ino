#define NO_PWM_INPUT_SUPPORT
#include <PWMDevices.h>

#define MIN 980
#define MED 1500 
#define MAX 1950
 
PWMDeviceOutput motor1;
PWMDeviceOutput motor2;
PWMDeviceOutput motor3;
PWMDeviceOutput motor4;

void setup() {
  Serial.begin(115200);
  
  motor1.attach(3);
  motor2.attach(9);
  motor3.attach(10);
  motor4.attach(11);

  motor1.write(MIN);
  motor2.write(MIN);
  motor3.write(MIN);
  motor4.write(MIN);
  delay(1000);

  motor3.write(MIN + 200);
  delay(1000);

  motor1.write(MIN);
  motor2.write(MIN);
  motor3.write(MIN);
  motor4.write(MIN);
  delay(1000);
}

void loop() {
  
}
