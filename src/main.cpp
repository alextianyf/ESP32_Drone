#include <Arduino.h>
#include <FSI6AB.h>
#include "Motor.h"
#include "MPU6050.h"
#include "PID.h"

void setup() {
  Serial.begin(115200);
  Receiver_Init();
  Motor_Init();
  gyro_init();
  LoopTimer = micros();
}

void loop() {
  Read_RC_Values();
  readGyro_signals();
  Motor_Start();
  //pid_to_pwm();
  delay(5);
}

