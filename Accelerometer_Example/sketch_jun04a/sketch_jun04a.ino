// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include <I2C.h>
#include <SD.h>

File myFile;
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int count = 0;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  int x = 0;
  Wire.write(x);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission();
  Serial.begin(9600);
  //xbee.setSerial(Serial);
  //XBee.begin(9600);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  if (!SD.begin(10)) {
    //Serial.println("initialization failed!");
    return;
   }
  //Serial.println("initialization done.");
   myFile = SD.open( "Data.csv", FILE_WRITE);
}
void loop() {
 
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.requestFrom(MPU_addr, 14); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L;
  Wire.endTransmission();
  delay(25);
  count ++;
   if (myFile) {
    //Serial.print("Writing to test.txt...");
    myFile.print(millis());
    myFile.print(",");
    myFile.print(AcX);
    myFile.print(",");
    myFile.print(AcY);
    myFile.print(",");
    myFile.print(AcZ);
    myFile.print(",");
    myFile.print(GyX);
    myFile.print(",");
    myFile.print(GyY);
    myFile.print(",");
    myFile.println(GyZ);
  }
  if (count >= 500) {  
    myFile.close();
    myFile = SD.open( "Data.csv", FILE_WRITE);
    count = 0;
  }
}
