#include <Arduino.h>
#include <ESP32Servo.h>
#include <FSI6AB.h>

Servo Throttle;

void Motor_Init(){
    pinMode(26,OUTPUT);
    Throttle.attach(26,1000,2000);
    //ledcSetup(0, 50, 12);
    //ledcAttachPin(26,0);
}

void Motor_Start(){
    int ThrottleValue = RC_VALUES[2];
    //ledcWrite(0,ThrottleValue);
    ThrottleValue = map(ThrottleValue,1050,2000,0,180);
    Throttle.write(ThrottleValue);
}