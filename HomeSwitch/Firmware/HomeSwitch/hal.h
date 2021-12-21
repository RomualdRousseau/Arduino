#include "Scheduler.h"
#include "Mqtt.h"
#include "SSR.h"

class _HAL_t
{
public:
  Scheduler scheduler;
  Mqtt      mqtt;
  SSR       ssr;

  void init();

private:
  void start_serial();

  void start_wifi();
};
static _HAL_t HAL;

void _HAL_t::init()
{
  start_serial();
  delay(10);
  start_wifi();
  mqtt.init();
  ssr.init();
}

void _HAL_t::start_serial() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println();
}

void _HAL_t::start_wifi()
{
  Serial.print("WiFi connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.println("WiFi connected");
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
