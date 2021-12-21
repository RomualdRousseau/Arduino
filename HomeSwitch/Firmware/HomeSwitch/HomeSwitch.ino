#include "hal_config.h"
#include "hal.h"

void listen_mqtt()
{
  HAL.mqtt.listen();
}

void update_mqtt()
{
  HAL.ssr.onoff(HAL.mqtt.value);
  HAL.mqtt.publish();
}

void setup()
{
  HAL.init();

  HAL.scheduler.register_mainloop(listen_mqtt);
  HAL.scheduler.register_timer(2000, update_mqtt);
}

void loop()
{
  HAL.scheduler.loop();
}
