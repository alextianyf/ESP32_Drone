#include <Arduino.h>
#include <IBusBM.h>
#include <ESP32Servo.h>
// IBusBM ibus;

// int potValue;

// Servo ESC;

// int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){
//   uint16_t ch = ibus.readChannel(channelInput);
//   if(ch<100) return defaultValue;
//   return map(ch, 1000, 2000, minLimit, maxLimit);
// }

// bool ReadSwitch(byte channelInput, bool defaultValue){
//   int intDefaultValue = (defaultValue)?100:0;
//   int ch = readChannel(channelInput,0,100,intDefaultValue);
//   return (ch>50);
// }

// void setup() {
//   pinMode(A0,INPUT);
//   Serial.begin(115200);
//   ibus.begin(Serial);
//   //ESC.attach(9,1000,2000);
// }

// void loop() {
//   for(byte i = 0; i<5;i++){
//     int value = readChannel(i,-100,100,0);
//     Serial.print("Ch ");
//     Serial.print(i+1);
//     Serial.print(":  ");
//     Serial.print(value);
//     Serial.print("  ");
//   }
//   Serial.print("Ch6: ");
//   Serial.print(ReadSwitch(5,false));
//   Serial.println();

//   //potValue = analogRead(A0);
//   //potValue = constrain(potValue,550,1023);
//   //int motorspeed  = map(potValue, 550, 1023,0,180);
//   //ESC.write(motorspeed);
//   //Serial.println(motorspeed);
//   //Serial.println(potValue);
//   delay(10);
// }

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("test");
  delay(1000) ;
}

