/**************************************************************************/
/*!
    @file     PWMDevices.h
    @author   Romuald Rousseau
    @license  BSD (see license.txt)

    Write and Read PWM signals

    This is a library to write and read PWM signals

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/
#ifndef PWMDEVICES_h
#define PWMDEVICES_h

#define PWM_MAX_CHANNELS	8

#ifndef NO_PWM_OUTPUT_SUPPORT
#include <Servo.h>

class PWMDeviceOutput
{
public:
  void attach(int pin)
  {
    _esc.attach(pin);
  }

  void write(int us)
  {
    _esc.writeMicroseconds(us);
  }

private:
  Servo _esc;
};
#endif

#ifndef NO_PWM_INPUT_SUPPORT
class PWMDeviceInput
{
public:
  void attach(int pin);
  
  int read();

private:
  int _channel_id;
};
#endif

#endif

