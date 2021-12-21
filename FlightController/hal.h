#include <Scheduler.h>
#include "Motion.h"
#include "Motor.h"
#include "Receiver.h"

class _HAL_t
{
public:
  Scheduler         scheduler;
  Motion            motion;
  Receiver          receiver;
  Motor             motor;
  
  void init();
};

static _HAL_t HAL;

void _HAL_t::init()
{
  randomSeed(analogRead(0));
  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("HAL Arduino v1.0");

  motion.init();

  receiver.init();

  motor.init();

  delay(1000);
}
