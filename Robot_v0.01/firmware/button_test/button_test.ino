
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// BUTTON PINS
#define BTN_UP     4
#define BTN_RIGHT  5
#define BTN_DOWN   6
#define BTN_LEFT   7

// LCD PINS - UNCHANGED
#define TFT_BL    9
#define TFT_CS    10
#define TFT_DC    11
#define TFT_RST   12
#define TFT_MOSI  13
#define TFT_SCLK  14

Adafruit_ST7735 tft(
  TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST
);

uint16_t BLUE, PINK, RED;

// FACE FUNCTIONS

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

int lastButton = -1;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLDOWN);
  pinMode(BTN_RIGHT, INPUT_PULLDOWN);
  pinMode(BTN_DOWN, INPUT_PULLDOWN);
  pinMode(BTN_LEFT, INPUT_PULLDOWN);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);

  BLUE = tft.color565(50, 180, 255);
  PINK = tft.color565(255, 100, 170);
  RED = tft.color565(255, 55, 55);

  // Backlight GPIO 9 remains reserved.
  // Do not enable direct GPIO drive until
  // the LCD backlight current is verified.

  neutral();

  Serial.println("LCD button test ready!");
}

void loop() {
  int pressed = -1;

  if (digitalRead(BTN_UP) == HIGH) {
    pressed = BTN_UP;
  }
  else if (digitalRead(BTN_RIGHT) == HIGH) {
    pressed = BTN_RIGHT;
  }
  else if (digitalRead(BTN_DOWN) == HIGH) {
    pressed = BTN_DOWN;
  }
  else if (digitalRead(BTN_LEFT) == HIGH) {
    pressed = BTN_LEFT;
  }

  // Only redraw when a different button
  // is pressed, not continuously.
  if (pressed != -1 && pressed != lastButton) {

    switch (pressed) {
      case BTN_UP:
        happy();
        Serial.println("UP - HAPPY");
        break;

      case BTN_RIGHT:
        confused();
        Serial.println("RIGHT - CONFUSED");
        break;

      case BTN_DOWN:
        sad();
        Serial.println("DOWN - SAD");
        break;

      case BTN_LEFT:
        angry();
        Serial.println("LEFT - ANGRY");
        break;
    }
  }

  lastButton = pressed;
  delay(40);
}
