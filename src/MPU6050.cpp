#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
sensors_event_t acc, gcc, temp;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

//#define GYRO_DEBUG 1

void gyro_init(){
    if(!mpu.begin()){
        Serial.print("Gyro init failed");
    }
    Serial.print("Gyro init successful");
}

void readGyro_signals(){
    mpu.getEvent(&acc, &gcc, &temp);
    // gcc gives radient and need to convert to degree
    // gyro value * 180 / 3.14
    GyX = ((gcc.gyro.x)*57.3);
    GyY = ((gcc.gyro.y)*57.3);
    GyZ = ((gcc.gyro.z)*57.3);
    AcX = acc.acceleration.x;
    AcY = acc.acceleration.y;
    AcZ = acc.acceleration.z;
    Tmp = temp.temperature;

    #ifdef GYRO_DEBUG
    Serial.print("Roll degree = "); Serial.print(GyX);
    Serial.print("; Pitch degree = "); Serial.print(GyY); 
    Serial.print("; Yaw degree = "); Serial.print(GyZ);
    Serial.print("; X-acc = "); Serial.print(AcX);
    Serial.print("; Y-acc = "); Serial.print(AcY);
    Serial.print("; Z-acc = "); Serial.print(AcZ);
    Serial.print("; Temperature = "); Serial.println(GyZ);
    #endif
}
