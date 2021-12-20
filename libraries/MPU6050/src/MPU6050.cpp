#include <Arduino.h>
#include <I2CDevices.h>

#include "MPU6050.h"

void MPU6050::begin(int scale, int range)
{
	I2CDevices.begin();

	I2CDevices.write_register_bit(MPU6050_I2C_ADDRESS, MPU6050_REG_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, 0);
	delay(100);

	I2CDevices.write_register_bits(MPU6050_I2C_ADDRESS, MPU6050_REG_PWR_MGMT_1, MPU6050_CLOCK_MASK, MPU6050_CLOCK_BIT, MPU6050_CLOCK_PLL_ZGYRO);
	I2CDevices.write_register_bits(MPU6050_I2C_ADDRESS, MPU6050_REG_PWR_MGMT_2, MPU6050_CLOCK_MASK, MPU6050_CLOCK_BIT, MPU6050_CLOCK_INTERNAL_8MHZ);

	I2CDevices.write_register_bits(MPU6050_I2C_ADDRESS, MPU6050_REG_CONFIG, MPU6050_DLPF_MASK, MPU6050_DLPF_BIT, MPU6050_DLPF_BW_42);

	I2CDevices.write_register_bits(MPU6050_I2C_ADDRESS, MPU6050_REG_GYRO_CONFIG, MPU6050_SCALE_MASK, MPU6050_SCALE_BIT, scale);
	I2CDevices.write_register_bits(MPU6050_I2C_ADDRESS, MPU6050_REG_ACCEL_CONFIG, MPU6050_RANGE_MASK, MPU6050_RANGE_BIT, range);

	I2CDevices.write_register8(MPU6050_I2C_ADDRESS, MPU6050_REG_SMPLRT_DIV, 0); // Sample rate 1KHz
}

void MPU6050::calibrate(float* gyro_zero_values)
{
	byte data[6];
	long sum[3] = {0, 0, 0};

	for(int i = 0; i < MPU6050_CALIBRATE_SAMPLE; i++)
	{
		I2CDevices.read_raw_buffer(MPU6050_I2C_ADDRESS, MPU6050_REG_GYRO_XOUT_H, data, 6);
		sum[0] += ((data[0] << 8) | data[1]);
		sum[1] += ((data[2] << 8) | data[3]);
		sum[2] += ((data[4] << 8) | data[5]);
	}
	
	gyro_zero_values[0] = sum[0] / MPU6050_CALIBRATE_SAMPLE;
	gyro_zero_values[1] = sum[1] / MPU6050_CALIBRATE_SAMPLE;
	gyro_zero_values[2] = sum[2] / MPU6050_CALIBRATE_SAMPLE;
}

void MPU6050::read(int* gyro_values, int* accel_values)
{
	byte data[14];

	I2CDevices.read_raw_buffer(MPU6050_I2C_ADDRESS, MPU6050_REG_ACCEL_XOUT_H, data, 14);

	// swap 16 bit sensor data
	gyro_values[0] = ((data[8] << 8) | data[9]);
	gyro_values[1] = ((data[10] << 8) | data[11]);
	gyro_values[2] = ((data[12] << 8) | data[13]);

	accel_values[0] = ((data[0] << 8) | data[1]);
	accel_values[1] = ((data[2] << 8) | data[3]);
	accel_values[2] = ((data[4] << 8) | data[5]);
}

