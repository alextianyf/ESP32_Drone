#include <Arduino.h>
#define RC_NUM_CHANNELS 4
#define RC_CH1 0// roll
#define RC_CH2 1// pitch
#define RC_CH3 2// throttle
#define RC_CH4 3// yaw

#define RC_CH1_PIN 33
#define RC_CH2_PIN 32
#define RC_CH3_PIN 35
#define RC_CH4_PIN 34

#define RC_LIMIT_MAX 2000
#define RC_LIMIT_MIN 1000

//#define RC_RECEIVE_DEBUG 1


uint16_t RC_VALUES[RC_NUM_CHANNELS];
uint32_t RC_START[RC_NUM_CHANNELS];
volatile uint16_t RC_SHARED[RC_NUM_CHANNELS];


void Read_Channel(uint8_t channel, uint8_t inputPin){
  if(digitalRead(inputPin) == HIGH){
    RC_START[channel] = micros();
  }else{
    uint16_t rc_compare = (uint16_t)(micros() - RC_START[channel]);
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
  #ifdef RC_RECEIVE_DEBUG
    Serial.print(  RC_VALUES[RC_CH1]);  Serial.print(",");
    Serial.print(  RC_VALUES[RC_CH2]);  Serial.print(",");
    Serial.print(  RC_VALUES[RC_CH3]);  Serial.print(",");
    Serial.println(RC_VALUES[RC_CH4]); 
  #endif 
}

