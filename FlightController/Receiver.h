#include <PWMDevices.h>

class Receiver
{
public:
  int command[HAL_RECEIVER_MAX_CHANNELS];

  void init();

  void read_pilot_commands();

  int get_safe_command(int i);

  int get_polar_command(int i);

private:
  PWMDeviceInput _channel[HAL_RECEIVER_MAX_CHANNELS];
};

void Receiver::init()
{
  Serial.println("Initializing receiver ... ");
    
  _channel[0].attach(HAL_RECEIVER_PIN1);
  _channel[1].attach(HAL_RECEIVER_PIN2);
  _channel[2].attach(HAL_RECEIVER_PIN3);
  _channel[3].attach(HAL_RECEIVER_PIN4);
  _channel[4].attach(HAL_RECEIVER_PIN5);
  _channel[5].attach(HAL_RECEIVER_PIN6);

  command[0] = HAL_RECEIVER_MINCMD;
  for(int i = 1; i < HAL_RECEIVER_MAX_CHANNELS; i++)
  {
    command[i] = HAL_RECEIVER_MIDCMD;
  }
}

void Receiver::read_pilot_commands()
{
  for(int i = 0; i < HAL_RECEIVER_MAX_CHANNELS; i++)
  {
    command[i] = (command[i] * 3 + _channel[i].read()) / 4;
  }
}

int Receiver::get_safe_command(int i)
{
  return map(command[i], HAL_RECEIVER_MINCMD, HAL_RECEIVER_MAXCMD, HAL_RECEIVER_MINCMD, HAL_RECEIVER_MAXCMD - HAL_RECEIVER_SAFETY);
}

int Receiver::get_polar_command(int i)
{
  return map(command[i], HAL_RECEIVER_MINCMD, HAL_RECEIVER_MAXCMD, 45, -45);
}
