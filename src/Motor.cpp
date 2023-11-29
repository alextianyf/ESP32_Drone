#include <Arduino.h>
#include <ESP32Servo.h>
#include <FSI6AB.h>
#include "MPU6050.h"
#include "PID.h"

//#define MOTOR_DEBUG 1

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
    // int ThrottleValue = inputThrottle;
    // ThrottleValue = map(ThrottleValue,1050,2000,0,180);
    // Serial.println(ThrottleValue);
    // F_L_Motor.write(ThrottleValue);
    pid_to_pwm();
    FLpwm = map(FLpwm,1050,2000,0,180);
    FRpwm = map(FRpwm,1050,2000,0,180);
    BLpwm = map(BLpwm,1050,2000,0,180);
    BRpwm = map(BRpwm,1050,2000,0,180);
    #ifdef MOTOR_DEBUG
        Serial.print("Front Left: "); Serial.print(FLpwm); Serial.print(" | "); Serial.print("Front Right: "); Serial.println(FRpwm);
        Serial.print("Back Left: "); Serial.print(BLpwm); Serial.print(" | ");Serial.print("Back Right: "); Serial.println(BRpwm); 
    #endif
    F_L_Motor.write(FLpwm);
    F_R_Motor.write(FRpwm);
    B_L_Motor.write(BLpwm);
    B_R_Motor.write(BRpwm);
}

