#ifndef PID_H_
#define PID_H_
#include <Arduino.h>

extern uint32_t LoopTimer;
void pid_to_pwm();
extern int FLpwm, FRpwm, BLpwm, BRpwm;

#endif 