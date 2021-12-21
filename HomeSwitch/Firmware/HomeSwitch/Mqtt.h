#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class Mqtt
{
  public:
    static int value;

    Mqtt();

    void init();

    void listen();

    void publish();

  private:
    void reconnect();

    static void callback(char* topic, byte* payload, unsigned int length);

    WiFiClient espClient;
    PubSubClient client;
};

int Mqtt::value = 0;

Mqtt::Mqtt():
  espClient(),
  client(espClient)
{
  Mqtt::value = 0;
}

void Mqtt::init()
{
  randomSeed(micros());
  client.setServer(mqtt_server, 1883);
  client.setCallback(Mqtt::callback);
}

void Mqtt::listen()
{
  while (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void Mqtt::publish()
{
  char msg[2];
  snprintf(msg, 2, "%ld", Mqtt::value);

  Serial.print(status_topic);
  Serial.print(": ");
  Serial.println(msg);

  client.publish(status_topic, msg);
}

void Mqtt::reconnect() {
  char clientId[20];
  snprintf(clientId, 20, "HomeSwitch-%04x", random(0xffff));
  
  Serial.print("MQTT connecting to ");
  Serial.println(mqtt_server);
  Serial.println("...");

  if (client.connect(clientId, mqtt_user, mqtt_password)) {
    Serial.println("MQTT connected");
    client.subscribe(action_topic);
    Mqtt::value = 0;
    publish();
  } else {
    Serial.print("MQTT failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
  }
}

void Mqtt::callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print(topic);
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
  }
  
  Mqtt::value = (payload[0] == '1') ? 1 : 0;
}
