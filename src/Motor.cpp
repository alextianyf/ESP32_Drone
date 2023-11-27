#include <Arduino.h>
#include <ESP32Servo.h>
#include <FSI6AB.h>
#include "MPU6050.h"

Servo F_L_Motor;//Front left Motor
Servo F_R_Motor;//Front right Motor
Servo B_L_Motor;//Back left Motor
Servo B_R_Motor;//Back right Motor

void Motor_Init(){
    pinMode(26,OUTPUT);//Front Right Motor
    pinMode(25,OUTPUT);//Front Left Motor
    pinMode(27,OUTPUT);//Back Left Motor
    pinMode(14,OUTPUT);//Back Right Motor
    F_R_Motor.attach(26,1000,2000);//Front Right Motor
    F_L_Motor.attach(25,1000,2000);//Front Left Motor
    B_L_Motor.attach(27,1000,2000);//Back Left Motor
    B_R_Motor.attach(14,1000,2000);//Back Right Motor
}

void Motor_Start(){
    int ThrottleValue = inputThrottle;
    ThrottleValue = map(ThrottleValue,1050,2000,0,180);
    F_L_Motor.write(ThrottleValue);
    F_R_Motor.write(ThrottleValue);
    B_L_Motor.write(ThrottleValue);
    B_R_Motor.write(ThrottleValue);
}

