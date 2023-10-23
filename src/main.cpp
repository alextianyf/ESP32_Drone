#include <Arduino.h>
#include <ESP32Servo.h>
#include <FSI6AB.h>

void setup() {
  Serial.begin(115200);
  Receiver_Init();
  delay(3000);
}

void loop() {
  Read_RC_Values();
}

