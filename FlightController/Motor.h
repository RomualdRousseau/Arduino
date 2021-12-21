#include "PWMDevices.h"

class Motor
{
public:
  void init();
  
  void attach(int pin);

  void calibrate();

  void set_zero_throttle();
  
  void set_throttle(int throttle, int pitch);

private:
  PWMDeviceOutput _motor[2];
};

void Motor::init()
{
  Serial.println("Initializing motors ... ");
  
  _motor[0].attach(HAL_MOTOR_PIN1);
  _motor[0].write(HAL_MOTOR_MINCMD);
  
  _motor[1].attach(HAL_MOTOR_PIN2);
  _motor[1].write(HAL_MOTOR_MINCMD);

  delay(2000);
}

void Motor::calibrate()
{
  Serial.println("Put battery for calibration ...");
  _motor[0].write(HAL_MOTOR_MAXCMD);
  _motor[1].write(HAL_MOTOR_MAXCMD);
  delay(5000);
  Serial.println("Calibration step 1 ...");
  delay(2000);
  
  Serial.println("Calibration step 2 ...");
  _motor[0].write(HAL_MOTOR_MINCMD);
  _motor[1].write(HAL_MOTOR_MINCMD);
  delay(2000);
  Serial.println("Calibration done.");
}

void Motor::set_throttle(int throttle, int pitch)
{
  _motor[0].write(constrain(throttle - pitch, HAL_MOTOR_MINCMD, HAL_MOTOR_MAXCMD));
  _motor[1].write(constrain(throttle + pitch, HAL_MOTOR_MINCMD, HAL_MOTOR_MAXCMD));
}

