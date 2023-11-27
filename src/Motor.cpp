#include <Arduino.h>
#include <ESP32Servo.h>
#include <FSI6AB.h>

Servo Motor1;//top left
Servo Motor2;//top right
Servo Motor3;
Servo Motor4;

void Motor_Init(){
    pinMode(26,OUTPUT);//Front Right Motor
    pinMode(25,OUTPUT);//Front Left Motor
    pinMode(27,OUTPUT);//Back Left Motor
    pinMode(14,OUTPUT);//Back Right Motor
    Motor1.attach(26,1000,2000);//Front Right Motor
    Motor2.attach(25,1000,2000);//Front Left Motor
    Motor3.attach(27,1000,2000);//Back Left Motor
    Motor4.attach(14,1000,2000);//Back Right Motor
}

void Motor_Start(){
    int ThrottleValue = RC_VALUES[2];
    ThrottleValue = map(ThrottleValue,1050,2000,0,180);
    Motor1.write(ThrottleValue);
    Motor2.write(ThrottleValue);
    Motor3.write(ThrottleValue);
    Motor4.write(ThrottleValue);
}