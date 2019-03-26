#include <SD.h>
#include <I2C.h>
#include <Timer.h>

File IMUData;
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
Timer IMURead;
Timer cycleSD;

void setup() {
  I2c.begin();
  I2c.setSpeed(1);
  I2c.timeOut(500);
  I2c.write(MPU_addr, 0x6B, 0);  // PWR_MGMT_1 register
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  if (!SD.begin(10)) {
    return;
   }
   IMUData = SD.open( "Data.csv", FILE_WRITE);
   IMURead.every(5, logData);
   cycleSD.every(10000, cycleSDCard);
   
}
void loop() {
  IMURead.update();
  cycleSD.update(); 
}

void logData() {
 
  //Wire.beginTransmission(MPU_addr);
  //I2c.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  //Wire.requestFrom(MPU_addr, 14); // request a total of 14 registers
  I2c.read(MPU_addr, 0x3B, 10000);
  AcX = I2c.receive() << 8 | I2c.receive(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcY =  I2c.receive() << 8 |  I2c.receive(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ =  I2c.receive() << 8 |  I2c.receive(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp =  I2c.receive() << 8 |  I2c.receive(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX =  I2c.receive() << 8 |  I2c.receive(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY =  I2c.receive() << 8 |  I2c.receive(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ =  I2c.receive() << 8 |  I2c.receive(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L;
  //Serial.println(I2c.read(0x3B, 1000, AcX));
  //AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L
   if (IMUData) {
    //Serial.print("Writing to test.txt...");
    IMUData.print(millis());
    IMUData.print(",");
    IMUData.print(AcX);
    IMUData.print(",");
    IMUData.print(AcY);
    IMUData.print(",");
    IMUData.print(AcZ);
    IMUData.print(",");
    IMUData.print(Tmp);
    IMUData.print(",");
    IMUData.print(GyX);
    IMUData.print(",");
    IMUData.print(GyY);
    IMUData.print(",");
    IMUData.println(GyZ);
  }
}
void cycleSDCard() { 
    IMUData.close();
    IMUData = SD.open( "Data.csv", FILE_WRITE);
}
