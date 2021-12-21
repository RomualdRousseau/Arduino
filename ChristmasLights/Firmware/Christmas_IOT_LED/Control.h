#include <SoftwareSerial.h>

SoftwareSerial mySerial(CONTROL_RX_PIN, CONTROL_TX_PIN);

class Control
{
public:
  boolean light_is_on;
  
  void init();
  
  void read_events();

private:
  char command_buffer[CONTROL_BUFFER_SIZE];
  int command_length; 
};

void Control::init()
{
  Serial.println("Initializing serial control ... ");

  mySerial.begin(CONTROL_SERIAL_SPEED);

  light_is_on = false;

  command_buffer[0] = 0;
  command_length = 0;
}

void Control::read_events()
{
  if (mySerial.available() > 0)
  {
    char c = (char) mySerial.read();
    if (c == '\n')
    {
      if(strcmp(command_buffer, "livingroom.light.status: 0") == 0)
      {
        light_is_on = false;
      }
      else if(strcmp(command_buffer, "livingroom.light.status: 1") == 0)
      {
        light_is_on = true;
      }
      
      command_buffer[0] = 0;
      command_length = 0;
    }
    else if (c != '\r')
    {
      command_buffer[command_length++] = c;
      command_buffer[command_length] = 0;
      
      if (command_length >= (CONTROL_BUFFER_SIZE - 1)) {
        command_buffer[0] = 0;
        command_length = 0;
      }
    }
  }
}
