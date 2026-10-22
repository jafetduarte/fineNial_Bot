
#include <Arduino.h>
#include <SPI.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Buttonss
const int BTN_UP = 4;
const int BTN_RIGHT = 5;
const int BTN_DOWN = 6;
const int BTN_LEFT = 7;

// LCD
const int TFT_BL = 9;
const int TFT_CS = 10;
const int TFT_DC = 11;
const int TFT_RST = 12;
const int TFT_MOSI = 13;
const int TFT_SCLK = 14;

// Servos
const int TILT_PIN = 16;
const int PAN_PIN = 17;

Adafruit_ST7735 tft(
  TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST
);

Servo tilt;
Servo pan;

int tiltAngle = 90;
int panAngle = 90;

const int MIN_ANGLE = 45;
const int MAX_ANGLE = 135;
const int STEP = 5;

uint16_t BLUE, PINK, RED;

void lookAt(int panPosition, int tiltPosition) {
  panAngle = constrain(panPosition, MIN_ANGLE, MAX_ANGLE);
  tiltAngle = constrain(tiltPosition, MIN_ANGLE, MAX_ANGLE);

  pan.write(panAngle);
  tilt.write(tiltAngle);
}

void neutral() {
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(31, 47, 17, 32, 7, BLUE);
  tft.fillRoundRect(80, 47, 17, 32, 7, BLUE);
  tft.fillRoundRect(54, 101, 20, 4, 2, PINK);
}

void happy() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawLine(25, 65, 38, 49, BLUE);
  tft.drawLine(38, 49, 51, 65, BLUE);
  tft.drawLine(77, 65, 90, 49, BLUE);
  tft.drawLine(90, 49, 103, 65, BLUE);
  tft.fillRoundRect(45, 96, 38, 15, 7, PINK);
}

void sad() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawLine(25, 54, 50, 69, BLUE);
  tft.drawLine(78, 69, 103, 54, BLUE);
  tft.drawLine(48, 108, 64, 96, PINK);
  tft.drawLine(64, 96, 80, 108, PINK);
}

void angry() {
  tft.fillScreen(ST77XX_BLACK);
  tft.fillTriangle(23, 45, 52, 66, 48, 75, RED);
  tft.fillTriangle(105, 45, 76, 66, 80, 75, RED);
  tft.fillRoundRect(46, 101, 36, 5, 2, RED);
}

void confused() {
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(29, 45, 20, 35, 8, BLUE);
  tft.fillRoundRect(83, 56, 12, 20, 6, BLUE);
  tft.drawLine(51, 105, 65, 99, PINK);
  tft.drawLine(65, 99, 79, 105, PINK);
}

void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLDOWN);
  pinMode(BTN_RIGHT, INPUT_PULLDOWN);
  pinMode(BTN_DOWN, INPUT_PULLDOWN);
  pinMode(BTN_LEFT, INPUT_PULLDOWN);

  tilt.setPeriodHertz(50);
  pan.setPeriodHertz(50);

  tilt.attach(TILT_PIN, 500, 2400);
  pan.attach(PAN_PIN, 500, 2400);

  lookAt(90, 90);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);

  BLUE = tft.color565(50, 180, 255);
  PINK = tft.color565(255, 100, 170);
  RED = tft.color565(255, 55, 55);

  // Only enable after verifying BL current requirements.
  // ledcAttach(TFT_BL, 5000, 8);
  // ledcWrite(TFT_BL, 180);

  neutral();
  Serial.println("Robot ready!");
}

void loop() {
  if (digitalRead(BTN_UP) == HIGH) {
    lookAt(panAngle, tiltAngle + STEP);
    happy();
    delay(180);
  }
  else if (digitalRead(BTN_RIGHT) == HIGH) {
    lookAt(panAngle + STEP, tiltAngle);
    confused();
    delay(180);
  }
  else if (digitalRead(BTN_DOWN) == HIGH) {
    lookAt(panAngle, tiltAngle - STEP);
    sad();
    delay(180);
  }
  else if (digitalRead(BTN_LEFT) == HIGH) {
    lookAt(panAngle - STEP, tiltAngle);
    angry();
    delay(180);
  }
}
