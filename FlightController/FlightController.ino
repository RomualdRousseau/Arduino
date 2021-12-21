#include "hal_config.h"
#include "hal.h"
#include <YAPID.h>

#define TASK_100HZ   FREQ_TO_DEADLINE(100)
#define TASK_50HZ    FREQ_TO_DEADLINE(50)
#define TASK_10HZ    FREQ_TO_DEADLINE(10)
#define TASK_1HZ     FREQ_TO_DEADLINE(1)

float kinematics_angle[3];

YAPIDController stabilizePID(-300, 300, 0, 0, DECAY_NONE);
YAPIDController ratePID(-1000, 1000, -500, 500, DECAY_NONE);

void calculate_kinematics(float dt)
{
  // integrate gyro_rate
  kinematics_angle[0] += HAL.motion.gyro_rate[0] * dt;
  kinematics_angle[1] += HAL.motion.gyro_rate[1] * dt;
  kinematics_angle[2] += HAL.motion.gyro_rate[2] * dt;

  // convert acceleration vector to angles
  float ax = atan2(HAL.motion.accel_rate[1], sqrt(pow(HAL.motion.accel_rate[0], 2) + pow(HAL.motion.accel_rate[2], 2))) * 180 / M_PI;
  float ay = atan2(HAL.motion.accel_rate[0], sqrt(pow(HAL.motion.accel_rate[1], 2) + pow(HAL.motion.accel_rate[2], 2))) * 180 / M_PI;
  
  // complementary filter
  kinematics_angle[0] = kinematics_angle[0] * 0.96 + ax * 0.04;
  kinematics_angle[1] = kinematics_angle[1] * 0.96 + ay * 0.04; 
}

void process_critical_sensors(float dt)
{
  HAL.motion.measure_sensors();
}

void process_flight_control(float dt)
{
  HAL.motion.evaluate_sensors();
  calculate_kinematics(dt);

  float a = HAL.receiver.get_polar_command(1) - kinematics_angle[0];
  float b = stabilizePID.compute(a);
  int c = ratePID.compute(b - HAL.motion.gyro_rate[0]);
  
  HAL.motor.set_throttle(HAL.receiver.get_safe_command(0), c);
}

void process_command_control(float dt)
{
  HAL.receiver.read_pilot_commands();
}

Task tasks[] = {
  {process_critical_sensors, TASK_IDLE, 0},
  {process_flight_control, TASK_100HZ, 0},
  {process_command_control, TASK_50HZ, 0}
};

void setup()
{
  HAL.init();

  stabilizePID.setTunings(4.5, 0, 0);
  ratePID.setTunings(1.0, 0.5, 0.04);

  //HAL.motor.calibrate();

  HAL.scheduler.calibrate(&tasks[0], 100);
  HAL.scheduler.register_tasks(tasks, 3);
}

void loop()
{
  HAL.scheduler.run_mainloop();
}

