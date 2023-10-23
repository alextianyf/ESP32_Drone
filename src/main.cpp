#include <Arduino.h>
#include <FSI6AB.h>
#include "Motor.h"

void setup() {
  Serial.begin(115200);
  Receiver_Init();
  Motor_Init();
}

void loop() {
  Read_RC_Values();
  Motor_Start();
}

