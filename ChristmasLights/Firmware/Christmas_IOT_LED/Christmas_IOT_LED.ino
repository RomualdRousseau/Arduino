#include "hal_config.h"
#include "hal.h"
#include "Patterns.h"

void setup()
{
  HAL.init();
  installPatterns();
}

void loop()
{
  EVERY_N_MILLISECONDS(1000 / FRAMES_PER_SECOND)
  {
    if(HAL.control.light_is_on)
    {
      HAL.ledstrip.play();
    }
    else
    {
      HAL.ledstrip.stop();
    }
  }
  
  EVERY_N_MILLISECONDS(20)
  {
    if(HAL.control.light_is_on)
    {
      HAL.ledstrip.update();
    }
  }
  
  EVERY_N_SECONDS( 10 )
  {
    if(HAL.control.light_is_on)
    {
      HAL.ledstrip.nextPattern();
    }
  }
  
  HAL.control.read_events();
}
