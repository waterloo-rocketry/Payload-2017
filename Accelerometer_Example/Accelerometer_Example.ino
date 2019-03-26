// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
#include <SD.h>
#include <SoftwareSerial.h>
File myFile;

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(0, 1); // RX, TX
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t GyX_abs = 0;
int16_t GyY_abs = 0;
int16_t GyZ_abs = 0;
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  XBee.begin(9600);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  myFile = SD.open("Test.csv", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    //Serial.print("Writing to test.txt...");
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
    // close the file:
    myFile.close();
    //Serial.println("done.");
  }
  else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
  Serial.print("ACC ");
  Serial.print((AcX-368.546247)/16384 );
  Serial.print(" ");
  Serial.print((AcY-477.3830508)/16384);
  Serial.print(" ");
  Serial.print((AcZ-16371.97094-16384)/-16384);
  Serial.println(" ");
  //Serial.print("GYRO ");
  //Serial.print((GyX+550.9476082)/131.07);
  //Serial.print(" ");
  //Serial.print((GyY+196.0751708)/131.07);
  //Serial.print(" ");
  if ((GyX+550.9476082)/131.07 > 1 || (GyX+550.9476082)/131.07 < -1) {
    GyX_abs += (GyX+550.9476082)/131.07;
  }
  if ((GyY+196.0751708)/131.07 > 1 || (GyY+196.0751708)/131.07 < -1) {
    GyY_abs += (GyY+196.0751708)/131.07;
  }
  if ((GyZ+232.6503417)/131.07 > 1 || (GyZ+232.6503417)/131.07 < -1) {
    GyZ_abs += (GyZ+232.6503417)/131.07;
  }
  //Serial.println((GyZ+232.6503417)/131.07);
  Serial.print("GYRO ");
  Serial.print(GyX_abs);
  Serial.print(" ");
  Serial.print(GyY_abs);
  Serial.print(" ");
  Serial.print((GyZ+232.6503417)/131.07);
  Serial.println(" ");
  Serial.write("TEST");
  Serial.println("TEST2");
  //Serial.println(" ");
  delay(200);
  
}
