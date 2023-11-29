#include <Arduino.h>
#include "MPU6050.h"
#include <FSI6AB.h>

//#define PID_DEBUG 1

uint32_t LoopTimer;
float DesiredRateRoll, DesiredRatePitch,DesiredRateYaw;
float ErrorRateRoll, ErrorRatePitch, ErrorRateYaw;
float InputRoll, InputThrottle, InputPitch, InputYaw;
float PrevErrorRateRoll, PrevErrorRatePitch, PrevErrorRateYaw;
float PrevItermRateRoll, PrevItermRatePitch,PrevItermRateYaw;
float PIDReturn[]={0, 0, 0};
float PRateRoll=0.6 ; 
float PRatePitch=PRateRoll; 
float PRateYaw=2;
float IRateRoll=3.5 ; 
float IRatePitch=IRateRoll; 
float IRateYaw=12;
float DRateRoll=0.03 ; 
float DRatePitch=DRateRoll; 
float DRateYaw=0;
float MotorInput1, MotorInput2, MotorInput3, MotorInput4;
int FLpwm, FRpwm, BLpwm, BRpwm;

void pid_equation(float Error, float P, float I, float D, float PrevError, float PrevItherm){
    float Pterm = P*Error;
    float Iterm = PrevItherm + I*(Error+PrevError)*0.004/2;
    if(Iterm > 400) Iterm = 400;
    else if (Iterm < -400) Iterm = -400;
    float Dterm = D*(Error-PrevError)/0.004;
    float PIDoutput = Pterm + Iterm + Dterm;
    if(PIDoutput>400) PIDoutput = 400;
    else if(PIDoutput<-400) PIDoutput = -400;

    PIDReturn[0] = PIDoutput;
    PIDReturn[1] = Error;
    PIDReturn[2] = Iterm;
}

void reset_pid(void){
    PrevErrorRateRoll = 0;
    PrevErrorRatePitch = 0;
    PrevErrorRateYaw = 0;
    PrevItermRateRoll = 0;
    PrevItermRatePitch = 0;
    PrevItermRateYaw = 0;
}

void pid_to_pwm(void){
    DesiredRateRoll = 0.15*(inputRoll - 1500);
    DesiredRatePitch = 0.15*(inputRoll - 1500);
    DesiredRateYaw = 0.15*(inputYaw - 1500);

    ErrorRateRoll = DesiredRateRoll - Gyr_X_Calibrated;//roll
    ErrorRatePitch = DesiredRatePitch - Gyr_Y_Calibrated;//pitch
    ErrorRateYaw = DesiredRateYaw - Gyr_Z_Calibrated;

    pid_equation(ErrorRateRoll, PRateRoll, IRateRoll, DRateRoll, PrevErrorRateRoll, PrevItermRateRoll);
    inputRoll = PIDReturn[0];
    PrevErrorRateRoll = PIDReturn[1];
    PrevItermRateRoll = PIDReturn[2];
    
    pid_equation(ErrorRatePitch, PRatePitch, IRatePitch, DRatePitch, PrevErrorRatePitch, PrevItermRatePitch);
    inputPitch = PIDReturn[0];
    PrevErrorRatePitch = PIDReturn[1];
    PrevItermRatePitch = PIDReturn[2];
    
    pid_equation(ErrorRateYaw, PRateYaw, IRateYaw, DRateYaw, PrevErrorRateYaw, PrevItermRateYaw);
    inputYaw = PIDReturn[0];
    PrevErrorRateYaw = PIDReturn[1];
    PrevItermRateYaw = PIDReturn[2];

    if (inputThrottle > 1800) inputThrottle = 1800;

    MotorInput1 = 1.024*(inputThrottle-inputRoll-inputPitch-inputYaw);//FR
    MotorInput2 = 1.024*(inputThrottle-inputRoll+inputPitch+inputYaw);//BR
    MotorInput3 = 1.024*(inputThrottle+inputRoll+inputPitch-inputYaw);//BL
    MotorInput4 = 1.024*(inputThrottle+inputRoll-inputPitch+inputYaw);//FL
    
    if(MotorInput1 > 2000) MotorInput1 = 1999;
    if(MotorInput2 > 2000) MotorInput2 = 1999;
    if(MotorInput3 > 2000) MotorInput3 = 1999;
    if(MotorInput4 > 2000) MotorInput4 = 1999;

    int ThrottleIDLE = 1180;
    if(MotorInput1 < ThrottleIDLE) MotorInput1 = ThrottleIDLE;
    if(MotorInput2 < ThrottleIDLE) MotorInput2 = ThrottleIDLE;
    if(MotorInput3 < ThrottleIDLE) MotorInput3 = ThrottleIDLE;
    if(MotorInput4 < ThrottleIDLE) MotorInput4 = ThrottleIDLE;

    int ThrottleCutOff = 1000;
    if(inputThrottle < 1050){
        MotorInput1 = ThrottleCutOff;
        MotorInput2 = ThrottleCutOff;
        MotorInput3 = ThrottleCutOff;
        MotorInput4 = ThrottleCutOff;
        reset_pid();
    }

    // while(micros() - LoopTimer < 4000);
    // LoopTimer=micros();

    FLpwm = MotorInput4;
    FRpwm = MotorInput1;
    BLpwm = MotorInput3;
    BRpwm = MotorInput2;

    #ifdef PID_DEBUG
        Serial.print("Front Left: "); Serial.print(FLpwm); Serial.print(" | "); Serial.print("Front Right: "); Serial.println(FRpwm);
        Serial.print("Back Left: "); Serial.print(BLpwm); Serial.print(" | ");Serial.print("Back Right: "); Serial.println(BRpwm); 
    #endif // DEBUG

}