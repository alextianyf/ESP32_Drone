#include <Arduino.h>
#include <IBusBM.h>

IBusBM ibus;
int channel[4] = {0,0,0,0};

int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){
  uint16_t ch = ibus.readChannel(channelInput);
  if(ch<100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

bool ReadSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)?100:0;
  int ch = readChannel(channelInput,0,100,intDefaultValue);
  return (ch>50);
}

void IBus_Init() {
  pinMode(16,INPUT);//Using RX2 to read IBUS input at GPIO16
  ibus.begin(Serial2);
}

void readJoystick() {
  for(byte i = 0; i<4;i++){
    channel[i] = readChannel(i,-100,100,0);
    Serial.print("Ch ");
    Serial.print(i+1);
    Serial.print(":  ");
    Serial.print(channel[i]);
    Serial.print("  ");
  }
  Serial.println();
}