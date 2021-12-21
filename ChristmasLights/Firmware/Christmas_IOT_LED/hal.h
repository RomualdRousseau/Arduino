#include "Control.h"
#include "LedStrip.h"

class _HAL_t
{
public:
  Control       control;
  LedStrip      ledstrip;

  void init();
};
static _HAL_t HAL;

void _HAL_t::init()
{
  delay(3000);
  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Christmas IOT v1.0 @ Romuald Rousseau");

  control.init();
  ledstrip.init();
  
}
