#ifndef MPU6050_H_
#define MPU6050_H_

void gyro_init();
void readGyro_signals();

extern float elapsedTime, RateRoll, RatePitch, RateYaw;
extern float Total_angle_x, Total_angle_y;
extern int16_t Gyr_X_Calibrated, Gyr_Y_Calibrated, Gyr_Z_Calibrated;

#endif 