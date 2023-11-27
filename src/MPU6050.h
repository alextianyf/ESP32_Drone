#ifndef MPU6050_H_
#define MPU6050_H_

void gyro_init();
void readGyro_signals();

extern float RateRoll, RatePitch, RateYaw;

#endif 