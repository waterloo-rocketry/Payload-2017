float AccX = 0;
float AccY = 0;
float AccZ = 1;
float GyX = 0;
float GyY = 0;
int counter = 0;
int roll = 0;
int downRoll = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  if (counter >= 500 && counter <= 1500) {
      AccZ = (float)random(1, 100)/100 + 1;
      AccX = (float)random(1, 100)/700;
      AccY = (float)random(1, 100)/700;
      roll += random(-1, 4);
      GyX += random(-5,5);
      GyY += random(-5,5);
  }
  if (counter >= 1500 && counter <= 2000) {
      AccZ = 1;
      roll--;
  }
  if (counter == 2000) {
    counter = 0;
    AccX = 0;
    AccY = 0;
    AccZ = 1;
    roll = 0;
    downRoll = 1000;
  }
  counter++;
  Serial.print("ACC ");
  Serial.print(AccX);
  Serial.print(" ");
  Serial.print(AccY);
  Serial.print(" ");
  Serial.print(AccZ);
  Serial.println(" ");
  Serial.print("GYRO ");
  Serial.print(GyX);
  Serial.print(" ");
  Serial.print(GyY);
  Serial.print(" ");
  Serial.print(roll);
  Serial.println(" ");
  // put your main code here, to run repeatedly:

}
