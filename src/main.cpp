#include <Arduino.h>
#include <FSI6AB.h>
#include "Motor.h"
#include "MPU6050.h"
#include "PID.h"
#include "BMP280.h"
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Receiver_Init();
  Wire.begin();
  Motor_Init();
  gyro_init();
  BMP280_init();
  LoopTimer = micros();
}

void loop() {
  //Read_RC_Values();
  readGyro_signals();
  //Motor_Start();
  barometer_signals();
  // AltitudeBarometer-=AltitudeBarometerStartUp;
  // Serial.print("Altitude [cm]: ");
  // Serial.println(AltitudeBarometer);
  // readbmp280();
  delay(500);
}

