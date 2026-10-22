
#include <ESP32Servo.h>

const int PAN_PIN = 15;   // Changed from 17 to 15
const int TILT_PIN = 16;  // Unchanged

Servo pan;
Servo tilt;

void setup() {
  Serial.begin(115200);

  pan.setPeriodHertz(50);
  tilt.setPeriodHertz(50);

  pan.attach(PAN_PIN, 500, 2400);
  tilt.attach(TILT_PIN, 500, 2400);

  pan.write(90);
  tilt.write(90);

  delay(1500);
}

void loop() {
  Serial.println("Testing TILT");

  tilt.write(75);
  delay(1000);
  tilt.write(105);
  delay(1000);
  tilt.write(90);
  delay(1000);

  Serial.println("Testing PAN");

  pan.write(75);
  delay(1000);
  pan.write(105);
  delay(1000);
  pan.write(90);
  delay(2000);
}
