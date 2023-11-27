#include <Arduino.h>
#define RC_NUM_CHANNELS 4
#define RC_CH1 0// roll
#define RC_CH2 1// pitch
#define RC_CH3 2// throttle
#define RC_CH4 3// yaw

#define RC_CH1_PIN 33//Connect with CH1
#define RC_CH2_PIN 32//Connect with CH2
#define RC_CH3_PIN 35//Connect with CH3
#define RC_CH4_PIN 34//Connect with CH4

#define RC_LIMIT_MAX 2000
#define RC_LIMIT_MIN 1000

//#define RC_RECEIVE_DEBUG 1


int16_t RC_VALUES[RC_NUM_CHANNELS];
int32_t RC_START[RC_NUM_CHANNELS];
volatile int16_t RC_SHARED[RC_NUM_CHANNELS];
int16_t inputRoll, inputPitch, inputThrottle, inputYaw;


void Read_Channel(int8_t channel, int8_t inputPin){
  if(digitalRead(inputPin) == HIGH){
    RC_START[channel] = micros();
  }else{
    int16_t rc_compare = (int16_t)(micros() - RC_START[channel]);
    if      (rc_compare< 1000){RC_SHARED[channel] = 1000;}
    else if (rc_compare > 2000){RC_SHARED[channel] = 2000;}
    else    {RC_SHARED[channel] = rc_compare;}
  }
}

void IntRead_CH1_Handler(){
  Read_Channel(RC_CH1, RC_CH1_PIN);
}

void IntRead_CH2_Handler(){
  Read_Channel(RC_CH2, RC_CH2_PIN);
}

void IntRead_CH3_Handler(){
  Read_Channel(RC_CH3, RC_CH3_PIN);
}

void IntRead_CH4_Handler(){
  Read_Channel(RC_CH4, RC_CH4_PIN);
}

void Receiver_Init() {
  pinMode(RC_CH1_PIN,INPUT);
  pinMode(RC_CH2_PIN,INPUT);
  pinMode(RC_CH3_PIN,INPUT);
  pinMode(RC_CH3_PIN,INPUT);

  attachInterrupt(digitalPinToInterrupt(RC_CH1_PIN),IntRead_CH1_Handler,CHANGE);
  attachInterrupt(digitalPinToInterrupt(RC_CH2_PIN),IntRead_CH2_Handler,CHANGE);
  attachInterrupt(digitalPinToInterrupt(RC_CH3_PIN),IntRead_CH3_Handler,CHANGE);
  attachInterrupt(digitalPinToInterrupt(RC_CH4_PIN),IntRead_CH4_Handler,CHANGE);
}

void Read_RC_Values(){
  noInterrupts();
  memcpy(RC_VALUES,(const void *)RC_SHARED, sizeof(RC_SHARED));
  interrupts();
  inputRoll = RC_VALUES[RC_CH1];
  inputPitch = RC_VALUES[RC_CH2];
  inputThrottle = RC_VALUES[RC_CH3];
  inputYaw = RC_VALUES[RC_CH4];
  #ifdef RC_RECEIVE_DEBUG
    Serial.print(inputRoll);  Serial.print(",");
    Serial.print(inputPitch);  Serial.print(",");
    Serial.print(inputThrottle);  Serial.print(",");
    Serial.println(inputYaw); 
  #endif 
}

