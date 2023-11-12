#include <Arduino.h>
#include <ESP32Servo.h>
#include <FSI6AB.h>

Servo Motor1;//top left
Servo Motor2;//top right
Servo Motor3;
Servo Motor4;

void Motor_Init(){
    pinMode(26,OUTPUT);
    pinMode(25,OUTPUT);
    pinMode(27,OUTPUT);
    pinMode(14,OUTPUT);
    Motor1.attach(26,1000,2000);
    Motor2.attach(25,1000,2000);
    Motor3.attach(27,1000,2000);
    Motor4.attach(14,1000,2000);
    //ledcSetup(0, 50, 12);
    //ledcAttachPin(26,0);
}

void Motor_Start(){
    int ThrottleValue = RC_VALUES[2];
    //ledcWrite(0,ThrottleValue);
    ThrottleValue = map(ThrottleValue,1050,2000,0,180);
    Motor1.write(ThrottleValue);
    Motor2.write(ThrottleValue);
    Motor3.write(ThrottleValue);
    Motor4.write(ThrottleValue);
}