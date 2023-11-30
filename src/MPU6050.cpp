#include <Arduino.h>
#include <Wire.h>
#include "FSI6AB.h"
#include "Motor.h"

#define MPU6050_ADDR 0x68
#define GYRO_DEBUG 1

float elapsedTime, curTime, preTime;
int gyro_error = 0;
int16_t Gyr_rawX, Gyr_rawY, Gyr_rawZ;
int16_t Gyr_X_Calibrated, Gyr_Y_Calibrated, Gyr_Z_Calibrated;
float Gyro_angle_x, Gyro_angle_y;
float Gyro_raw_error_x, Gyro_raw_error_y, Gyro_raw_error_z;

//Acc Variables
int acc_error=0;                         //We use this variable to only calculate once the Acc data error
float rad_to_deg = 180/3.141592654;      //This value is for pasing from radians to degrees values
float Acc_rawX, Acc_rawY, Acc_rawZ;    //Here we store the raw data read 
float Acc_angle_x, Acc_angle_y;          //Here we store the angle value obtained with Acc data
float Acc_angle_error_x, Acc_angle_error_y; //Here we store the initial Acc data error

float Total_angle_x, Total_angle_y;

void gyro_init(){
    //Wire.begin();
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x1B);
    Wire.write(0x10);
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU6050_ADDR);            
    Wire.write(0x1A);  
    Wire.write(0x05);                      
    Wire.endTransmission(true);

    //Acc config
    Wire.beginTransmission(MPU6050_ADDR);   //Start communication with the address found during search.
    Wire.write(0x1C);                       //We want to write to the ACCEL_CONFIG register
    Wire.write(0x10);                       //Set the register bits as 00010000 (+/- 8g full scale range)
    Wire.endTransmission(true);

    curTime = millis();
    /*Here we calculate the acc data error before we start the loop
    * I make the mean of 200 values, that should be enough*/
    if(gyro_error == 0){
        for(int i = 0; i<200; i++){
            Wire.beginTransmission(MPU6050_ADDR);
            Wire.write(0x43);
            Wire.endTransmission(false);
            Wire.requestFrom(0x68, 6);

            Gyr_rawX = Wire.read()<<8 | Wire.read();
            Gyr_rawY = Wire.read()<<8 | Wire.read();
            Gyr_rawZ = Wire.read()<<8 | Wire.read();

            Gyro_raw_error_x = Gyro_raw_error_x + (Gyr_rawX/32.8);
            Gyro_raw_error_y = Gyro_raw_error_y + (Gyr_rawY/32.8);
            Gyro_raw_error_z = Gyro_raw_error_z + (Gyr_rawZ/32.8);

            if(i == 199){
                Gyro_raw_error_x = Gyro_raw_error_x/200;
                Gyro_raw_error_y = Gyro_raw_error_y/200;
                Gyro_raw_error_z = Gyro_raw_error_z/200;
                gyro_error=1;
            }
        }
    }

    if(acc_error==0)
    {
        for(int a=0; a<200; a++)
        {
        Wire.beginTransmission(0x68);
        Wire.write(0x3B);                       //Ask for the 0x3B register- correspond to AcX
        Wire.endTransmission(false);
        Wire.requestFrom(0x68,6); 
        
        Acc_rawX=(Wire.read()<<8|Wire.read())/4096.0 ; //each value needs two registres
        Acc_rawY=(Wire.read()<<8|Wire.read())/4096.0 ;
        Acc_rawZ=(Wire.read()<<8|Wire.read())/4096.0 ;

        /*---X---*/
        Acc_angle_error_x = Acc_angle_error_x + ((atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg));
        /*---Y---*/
        Acc_angle_error_y = Acc_angle_error_y + ((atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg)); 
        
        if(a==199)
        {
            Acc_angle_error_x = Acc_angle_error_x/200;
            Acc_angle_error_y = Acc_angle_error_y/200;
            acc_error=1;
        }
        }
    }//end of acc error calculation

}


void readGyro_signals(){
    preTime= curTime;
    curTime = millis();
    elapsedTime = (curTime - preTime) / 1000;
    
    //GYRO Config Setting
    //DLPF(Digital Low Pass Filter) setting for both Gyro and Accel
    //00000101 set Acce bandwidth 10Hz delya 13.8ms and Gyro bandwidth 10Hz and delay 13.4ms FS:1KHz
    // Wire.beginTransmission(MPU6050_ADDR);            
    // Wire.write(0x1A);  
    // Wire.write(0x05);                      
    // Wire.endTransmission(true);

    Wire.beginTransmission(MPU6050_ADDR);  //begin, Send the slave adress (in this case 68) 
    Wire.write(0x43);                      //First adress of the Gyro data
    Wire.endTransmission(false);
    Wire.requestFrom(0x68,6);           //We ask for just 4 registers
        
    Gyr_rawX=Wire.read()<<8|Wire.read();     //Once again we shif and sum
    Gyr_rawY=Wire.read()<<8|Wire.read();
    Gyr_rawZ=Wire.read()<<8|Wire.read();
    /*Now in order to obtain the gyro data in degrees/seconds we have to divide first
    the raw value by 32.8 because that's the value that the datasheet gives us for a 1000dps range*/
    /*---X---*/
    Gyr_X_Calibrated = (Gyr_rawX/32.8) - Gyro_raw_error_x; 
    /*---Y---*/
    Gyr_Y_Calibrated = (Gyr_rawY/32.8) - Gyro_raw_error_y;
    Gyr_Z_Calibrated = (Gyr_rawZ/32.8) - Gyro_raw_error_z;
    
    /*Now we integrate the raw value in degrees per seconds in order to obtain the angle
    * If you multiply degrees/seconds by seconds you obtain degrees */
    // /*---X---*/
    // Gyro_angle_x = Gyro_angle_x  + Gyr_rawX*elapsedTime;
    // /*---X---*/
    // Gyro_angle_y = Gyro_angle_y  + Gyr_rawY*elapsedTime;

    //////////////////////////////////////Acc read/////////////////////////////////////

    Wire.beginTransmission(0x68);     //begin, Send the slave adress (in this case 68) 
    Wire.write(0x3B);                 //Ask for the 0x3B register- correspond to AcX
    Wire.endTransmission(false);      //keep the transmission and next
    Wire.requestFrom(0x68,6);    //We ask for next 6 registers starting withj the 3B  
    /*We have asked for the 0x3B register. The IMU will send a brust of register.
    * The amount of register to read is specify in the requestFrom function.
    * In this case we request 6 registers. Each value of acceleration is made out of
    * two 8bits registers, low values and high values. For that we request the 6 of them  
    * and just make then sum of each pair. For that we shift to the left the high values 
    * register (<<) and make an or (|) operation to add the low values.
    If we read the datasheet, for a range of+-8g, we have to divide the raw values by 4096*/    
    Acc_rawX=(Wire.read()<<8|Wire.read())/4096.0 ; //each value needs two registres
    Acc_rawY=(Wire.read()<<8|Wire.read())/4096.0 ;
    Acc_rawZ=(Wire.read()<<8|Wire.read())/4096.0 ; 
    /*Now in order to obtain the Acc angles we use euler formula with acceleration values
    after that we substract the error value found before*/  
    /*---X---*/
    Acc_angle_x = (atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_x;
    /*---Y---*/
    Acc_angle_y = (atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg) - Acc_angle_error_y;    


    //////////////////////////////////////Total angle and filter/////////////////////////////////////
    /*---X axis angle---*/
    Total_angle_x = 0.98 *(Total_angle_x + Gyro_angle_x) + 0.02*Acc_angle_x;
    /*---Y axis angle---*/
    Total_angle_y = 0.98 *(Total_angle_y + Gyro_angle_y) + 0.02*Acc_angle_y;
    
    #ifdef GYRO_DEBUG
        Serial.print("GyroX Roll: "); Serial.print(Gyr_X_Calibrated); Serial.print("   |   ");
        Serial.print("GyroY Pitch: "); Serial.print(Gyr_Y_Calibrated); Serial.print("   |   ");
        Serial.print("GyroZ Yaw: "); Serial.println(Gyr_Z_Calibrated);
    #endif

}



