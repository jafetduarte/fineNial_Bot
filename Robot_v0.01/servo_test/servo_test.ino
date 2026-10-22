
#include <ESP32Servo.h>

const int TILT_PIN = 16;
const int PAN_PIN = 17;

Servo tilt;
Servo pan;

void setup() {
  Serial.begin(115200);

  tilt.setPeriodHertz(50);
  pan.setPeriodHertz(50);

  tilt.attach(TILT_PIN, 500, 2400);
  pan.attach(PAN_PIN, 500, 2400);

  tilt.write(90);
  pan.write(90);

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
