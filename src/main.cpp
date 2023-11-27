#include <Arduino.h>
#include <FSI6AB.h>
#include "Motor.h"
#include "MPU6050.h"

void setup() {
  Serial.begin(115200);
  Receiver_Init();
  Motor_Init();
  gyro_init();
}

void loop() {
  Read_RC_Values();
  Motor_Start();
  readGyro_signals();
  delay(50);
}

