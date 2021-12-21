#ifdef HAL_MOTION_MPU6050
#include "MPU6050.h"
#endif

class Motion
{
public:
  float   gyro_rate[3];
  float   accel_rate[3];

  void init();

  void measure_sensors();

  void evaluate_sensors();

private:
#ifdef HAL_MOTION_MPU6050
  MPU6050     _mpu;
#endif

  float       _gyro_zero[3] = { -281.00, 18.00, -83.00 };
  long        _gyro_sample[3] = { 0, 0, 0 };
  long        _accel_sample[3] = { 0, 0, 0 };
  int         _sample_count = 0;
};

void Motion::init()
{
  Serial.println("Initializing motion device ... ");
  _mpu.begin();

  Serial.println("Calibrating motion device ... ");
  _mpu.calibrate(_gyro_zero);      
}

void Motion::measure_sensors()
{
  int g[3];
  int a[3];

  _mpu.read(g, a);

  _gyro_sample[0] += g[0];
  _gyro_sample[1] += g[1];
  _gyro_sample[2] += g[2];
  
  _accel_sample[0] += a[0];
  _accel_sample[1] += a[1];
  _accel_sample[2] += a[2];

  _sample_count++;
}

void Motion::evaluate_sensors()
{
  float sample_factor = 1.0 / _sample_count;

  gyro_rate[0] = (_gyro_sample[0] * sample_factor - _gyro_zero[0]) * _mpu.gyro_scale_factor;
  gyro_rate[1] = (_gyro_sample[1] * sample_factor - _gyro_zero[1]) * _mpu.gyro_scale_factor;
  gyro_rate[2] = (_gyro_sample[2] * sample_factor - _gyro_zero[2]) * _mpu.gyro_scale_factor;
  
  accel_rate[0] = _accel_sample[0] * sample_factor;
  accel_rate[1] = _accel_sample[1] * sample_factor;
  accel_rate[2] = _accel_sample[2] * sample_factor;

  _gyro_sample[0] = 0;
  _gyro_sample[1] = 0;
  _gyro_sample[2] = 0;
  
  _accel_sample[0] = 0;
  _accel_sample[1] = 0;
  _accel_sample[2] = 0;
  
  _sample_count = 0;
}

