#include <Arduino.h>
#include <Wire.h>

//#define SCANI2C 1 
//#define ADAFRUIT_TEST 1
#define ALTITUDE_DEBUG 1

//Adafruit_BMP280 bmp;
#define BMP280_ADDRESS 0x76 // BMP280 I2C address
uint16_t dig_T1, dig_P1;
int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5;
int16_t dig_P6, dig_P7, dig_P8, dig_P9;

float AltitudeBarometer, AltitudeBarometerStartUp;
int RateCalibrationNumber;

#ifdef SCANI2C
void scanI2C() {
    byte error, address;
    int nDevices = 0;

    Serial.println("Scanning...");

    for (address = 1; address < 127; ++address) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
        Serial.print("Found device at address 0x");
        if (address < 16)
            Serial.print("0");
        Serial.print(address, HEX);
        Serial.println("!");

        ++nDevices;
        } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16)
            Serial.print("0");
        Serial.println(address, HEX);
        }
    }

    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
    Serial.println("done\n");
}
#endif

#ifdef ADAFRUIT_TEST
void BMP280_init(){
    //scanI2C();
    if (!bmp.begin(0x76)) {
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
        while (1);
    }
    Serial.println("Setup successfully");
}

void barometer_signals(){
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure() / 100.0F); // Pressure in hPa
    Serial.println(" hPa");
}
#endif

void BMP280_init(){
    Wire.beginTransmission(0x76);
    if(Wire.endTransmission() == 0){
        Wire.write(0xF4);
        //temp oversampling x2 & pressure oversampling x16
        Wire.write(0x57);
        Wire.endTransmission(true); 
        Serial.println("BMP280 oversampling config successfully");
    }else{
        Serial.println("BMP280 oversampling config failed");
        while(1);
    }
    Wire.beginTransmission(0x76);
    if(Wire.endTransmission() == 0){
        Wire.write(0xF5);
        //setting IIR filter
        Wire.write(0x14);
        Wire.endTransmission(true); 
        Serial.println("BMP280 IIR Filter config successfully");
    }else{
        Serial.println("BMP280 IIR Filter config failed");
        while(1);
    }

    uint8_t data[24],i=0; 
    Wire.beginTransmission(0x76);
    Wire.write(0x88);
    Wire.endTransmission(false);
    Wire.requestFrom(0x76,24);
    // for(uint8_t i = 0; i < 24 ; i++){
    //     data[i] = Wire.read();
    // }
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
    Serial.println("Trimming Parameter readout successfully");
    dig_T1 = (data[1] << 8) | data[0]; 
    dig_T2 = (data[3] << 8) | data[2];
    dig_T3 = (data[5] << 8) | data[4];
    dig_P1 = (data[7] << 8) | data[6]; 
    dig_P2 = (data[9] << 8) | data[8];
    dig_P3 = (data[11]<< 8) | data[10];
    dig_P4 = (data[13]<< 8) | data[12];
    dig_P5 = (data[15]<< 8) | data[14];
    dig_P6 = (data[17]<< 8) | data[16];
    dig_P7 = (data[19]<< 8) | data[18];
    dig_P8 = (data[21]<< 8) | data[20];
    dig_P9 = (data[23]<< 8) | data[22]; 

    for (RateCalibrationNumber=0; RateCalibrationNumber<2000;RateCalibrationNumber ++) {
        Wire.beginTransmission(0x76);
        Wire.write(0xF7);
        Wire.endTransmission(false);
        Wire.requestFrom(0x76,6);
        uint32_t press_msb = Wire.read();
        uint32_t press_lsb = Wire.read();
        uint32_t press_xlsb = Wire.read();
        uint32_t temp_msb = Wire.read();
        uint32_t temp_lsb = Wire.read();
        uint32_t temp_xlsb = Wire.read();
        
        // Construct the raw temperature and pressure measurements
        unsigned long int adc_P = (press_msb << 12) | (press_lsb << 4) | (press_xlsb >>4);
        unsigned long int adc_T = (temp_msb << 12) | (temp_lsb << 4) | (temp_xlsb >>4);
        
        //Construct the fine resolution temperature value
        signed long int var1, var2;
        var1 = ((((adc_T >> 3) - ((signed long int )dig_T1 <<1)))* ((signed long int )dig_T2)) >> 11;
        var2 = (((((adc_T >> 4) - ((signed long int )dig_T1)) * ((adc_T>>4) - ((signed long int )dig_T1)))>> 12) * ((signed long int )dig_T3)) >> 14;
        signed long int t_fine = var1 + var2;

        unsigned long int p;
        var1 = (((signed long int )t_fine)>>1) - (signed long int )64000;
        var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int )dig_P6);
        var2 = var2 + ((var1*((signed long int )dig_P5)) <<1);
        var2 = (var2>>2)+(((signed long int )dig_P4)<<16);
        var1 = (((dig_P3 * (((var1>>2)*(var1>>2)) >> 13))>>3)+((((signed long int )dig_P2) * var1)>>1))>>18;
        var1 = ((((32768+var1))*((signed long int )dig_P1)) >>15);
        
        if (var1 == 0) { p=0;} 
        p = (((unsigned long int )(((signed long int ) 1048576)-adc_P)-(var2>>12)))*3125;
        if(p<0x80000000){ p = (p << 1) / ((unsigned long int ) var1);}
        else { p = (p / (unsigned long int )var1) * 2; }
        var1 = (((signed long int )dig_P9) * ((signed long int ) (((p>>3) * (p>>3))>>13)))>>12;
        var2 = (((signed long int )(p>>2)) * ((signed long int )dig_P8))>>13;
        p = (unsigned long int)((signed long int )p + ((var1 + var2+ dig_P7) >> 4));
        double pressure=(double)p/100;
        AltitudeBarometer=44330*(1-pow(pressure/1013.25, 1/5.255))*100;
        AltitudeBarometerStartUp+=AltitudeBarometer;
        delay(1);
    }
    AltitudeBarometerStartUp/=2000;
}

void barometer_signals(){
    Wire.beginTransmission(0x76);
    Wire.write(0xF7);
    Wire.endTransmission(false);
    Wire.requestFrom(0x76,6);
    uint32_t press_msb = Wire.read();
    uint32_t press_lsb = Wire.read();
    uint32_t press_xlsb = Wire.read();
    uint32_t temp_msb = Wire.read();
    uint32_t temp_lsb = Wire.read();
    uint32_t temp_xlsb = Wire.read();
    
    // Construct the raw temperature and pressure measurements
    unsigned long int adc_P = (press_msb << 12) | (press_lsb << 4) | (press_xlsb >>4);
    unsigned long int adc_T = (temp_msb << 12) | (temp_lsb << 4) | (temp_xlsb >>4);
    
    //Construct the fine resolution temperature value
    signed long int var1, var2;
    var1 = ((((adc_T >> 3) - ((signed long int )dig_T1 <<1)))* ((signed long int )dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((signed long int )dig_T1)) * ((adc_T>>4) - ((signed long int )dig_T1)))>> 12) * ((signed long int )dig_T3)) >> 14;
    signed long int t_fine = var1 + var2;

    unsigned long int p;
    var1 = (((signed long int )t_fine)>>1) - (signed long int )64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11) * ((signed long int )dig_P6);
    var2 = var2 + ((var1*((signed long int )dig_P5)) <<1);
    var2 = (var2>>2)+(((signed long int )dig_P4)<<16);
    var1 = (((dig_P3 * (((var1>>2)*(var1>>2)) >> 13))>>3)+((((signed long int )dig_P2) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((signed long int )dig_P1)) >>15);
    
    if (var1 == 0) { p=0;} 
    p = (((unsigned long int )(((signed long int ) 1048576)-adc_P)-(var2>>12)))*3125;
    if(p<0x80000000){ p = (p << 1) / ((unsigned long int ) var1);}
    else { p = (p / (unsigned long int )var1) * 2; }
    var1 = (((signed long int )dig_P9) * ((signed long int ) (((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((signed long int )(p>>2)) * ((signed long int )dig_P8))>>13;
    p = (unsigned long int)((signed long int )p + ((var1 + var2+ dig_P7) >> 4));
    double pressure=(double)p/100;
    AltitudeBarometer=44330*(1-pow(pressure/1013.25, 1/5.255))*100;
    AltitudeBarometer-=AltitudeBarometerStartUp;
    #ifdef ALTITUDE_DEBUG
    Serial.print("Altitude[cm]:"); Serial.println(AltitudeBarometer);
    #endif // DEBUG

}