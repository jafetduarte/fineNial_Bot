
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// ==========================================
// ESP32-S3 DESK BUDDY V0.03
// DIRECT LEDC PWM - NO ESP32SERVO LIBRARY
// Requires Arduino ESP32 core 3.x
// ==========================================

// BUTTONS
const int BTN_UP    = 4;
const int BTN_RIGHT = 5;
const int BTN_DOWN  = 6;
const int BTN_LEFT  = 7;

// LCD
const int TFT_BL   = 9;
const int TFT_CS   = 10;
const int TFT_DC   = 11;
const int TFT_RST  = 12;
const int TFT_MOSI = 13;
const int TFT_SCLK = 14;

// SERVOS
const int PAN_PIN  = 15;
const int TILT_PIN = 16;

// LCD SOFTWARE SPI
Adafruit_ST7735 tft(
  TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST
);

// SERVO SETTINGS
const int SERVO_FREQ = 50;
const int PWM_BITS = 14;

// 50 Hz = 20,000 microseconds per cycle
const int SERVO_PERIOD_US = 20000;

// Starting pulse range for typical micro servos.
// Adjust only after confirming your servo's limits.
const int SERVO_MIN_US = 1000;
const int SERVO_MAX_US = 2000;

const int MIN_ANGLE = 45;
const int MAX_ANGLE = 135;
const int MOVE_STEP = 5;

int panAngle = 90;
int tiltAngle = 90;

bool panReady = false;
bool tiltReady = false;

// BUTTON TIMING
const unsigned long MOVE_INTERVAL = 180;
const unsigned long DEBUG_INTERVAL = 1000;

unsigned long lastMoveTime = 0;
unsigned long lastDebugTime = 0;

// FACE STATES
enum Face {
  NEUTRAL,
  HAPPY,
  SAD,
  ANGRY,
  CONFUSED
};

Face currentFace = NEUTRAL;

// COLORS
uint16_t BLUE;
uint16_t PINK;
uint16_t RED;

// ==========================================
// SERVO PWM FUNCTIONS
// ==========================================

// Convert a servo angle to LEDC duty cycle.
uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);

  uint32_t pulseUs = map(
    angle,
    0, 180,
    SERVO_MIN_US, SERVO_MAX_US
  );

  const uint32_t pwmLevels = 1UL << PWM_BITS;

  return (uint32_t)(
    (uint64_t)pulseUs * pwmLevels /
    SERVO_PERIOD_US
  );
}

void writeServo(int pin, int angle, bool ready) {
  if (!ready) return;

  uint32_t duty = angleToDuty(angle);
  ledcWrite(pin, duty);
}

void lookAt(int pan, int tilt) {
  panAngle = constrain(
    pan, MIN_ANGLE, MAX_ANGLE
  );

  tiltAngle = constrain(
    tilt, MIN_ANGLE, MAX_ANGLE
  );

  writeServo(PAN_PIN, panAngle, panReady);
  writeServo(TILT_PIN, tiltAngle, tiltReady);
}

void moveUp() {
  lookAt(panAngle, tiltAngle + MOVE_STEP);
}

void moveDown() {
  lookAt(panAngle, tiltAngle - MOVE_STEP);
}

void moveRight() {
  lookAt(panAngle + MOVE_STEP, tiltAngle);
}

void moveLeft() {
  lookAt(panAngle - MOVE_STEP, tiltAngle);
}

// ==========================================
// FACE DRAWING
// ==========================================

void drawFace(Face face) {
  tft.fillScreen(ST77XX_BLACK);

  switch (face) {

    case NEUTRAL:
      tft.fillRoundRect(
        31, 47, 17, 32, 7, BLUE
      );
      tft.fillRoundRect(
        80, 47, 17, 32, 7, BLUE
      );
      tft.fillRoundRect(
        54, 101, 20, 4, 2, PINK
      );
      break;

    case HAPPY:
      tft.drawLine(25, 65, 38, 49, BLUE);
      tft.drawLine(38, 49, 51, 65, BLUE);
      tft.drawLine(77, 65, 90, 49, BLUE);
      tft.drawLine(90, 49, 103, 65, BLUE);
      tft.fillRoundRect(
        45, 96, 38, 15, 7, PINK
      );
      break;

    case SAD:
      tft.drawLine(25, 54, 50, 69, BLUE);
      tft.drawLine(78, 69, 103, 54, BLUE);
      tft.drawLine(48, 108, 64, 96, PINK);
      tft.drawLine(64, 96, 80, 108, PINK);
      break;

    case ANGRY:
      tft.fillTriangle(
        23, 45, 52, 66, 48, 75, RED
      );
      tft.fillTriangle(
        105, 45, 76, 66, 80, 75, RED
      );
      tft.fillRoundRect(
        46, 101, 36, 5, 2, RED
      );
      break;

    case CONFUSED:
      tft.fillRoundRect(
        29, 45, 20, 35, 8, BLUE
      );
      tft.fillRoundRect(
        83, 56, 12, 20, 6, BLUE
      );
      tft.drawLine(51, 105, 65, 99, PINK);
      tft.drawLine(65, 99, 79, 105, PINK);
      break;
  }

  currentFace = face;
}

void setFace(Face face) {
  if (face != currentFace) {
    drawFace(face);
  }
}

// ==========================================
// BUTTON READING
// ==========================================

int readButton() {
  if (digitalRead(BTN_UP) == HIGH)
    return BTN_UP;

  if (digitalRead(BTN_RIGHT) == HIGH)
    return BTN_RIGHT;

  if (digitalRead(BTN_DOWN) == HIGH)
    return BTN_DOWN;

  if (digitalRead(BTN_LEFT) == HIGH)
    return BTN_LEFT;

  return -1;
}

// ==========================================
// SETUP
// ==========================================

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("==========================");
  Serial.println("DESK BUDDY V0.03 - LEDC");
  Serial.println("==========================");

  // BUTTONS
  pinMode(BTN_UP, INPUT_PULLDOWN);
  pinMode(BTN_RIGHT, INPUT_PULLDOWN);
  pinMode(BTN_DOWN, INPUT_PULLDOWN);
  pinMode(BTN_LEFT, INPUT_PULLDOWN);

  Serial.println("[OK] Buttons configured");

  // LCD
  Serial.println("[...] Initializing LCD");

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(0);

  BLUE = tft.color565(50, 180, 255);
  PINK = tft.color565(255, 100, 170);
  RED  = tft.color565(255, 55, 55);

  // Backlight GPIO 9 is reserved.
  // No backlight PWM until its electrical
  // requirements are verified.

  drawFace(NEUTRAL);

  Serial.println("[OK] LCD initialized");

  // SERVO PWM
  Serial.println("[...] Attaching PAN PWM");

  panReady = ledcAttach(
    PAN_PIN, SERVO_FREQ, PWM_BITS
  );

  Serial.print("PAN PWM attached: ");
  Serial.println(panReady ? "YES" : "NO");

  Serial.println("[...] Attaching TILT PWM");

  tiltReady = ledcAttach(
    TILT_PIN, SERVO_FREQ, PWM_BITS
  );

  Serial.print("TILT PWM attached: ");
  Serial.println(tiltReady ? "YES" : "NO");

  lookAt(90, 90);

  Serial.println("[READY] Robot running");
}

// ==========================================
// MAIN LOOP
// ==========================================

void loop() {
  unsigned long now = millis();

  // DIAGNOSTICS
  if (now - lastDebugTime >= DEBUG_INTERVAL) {
    lastDebugTime = now;

    Serial.printf(
      "Buttons: UP=%d RIGHT=%d DOWN=%d LEFT=%d\n",
      digitalRead(BTN_UP),
      digitalRead(BTN_RIGHT),
      digitalRead(BTN_DOWN),
      digitalRead(BTN_LEFT)
    );
  }

  static int previousButton = -1;
  int button = readButton();

  // CHANGE FACE
  if (button != previousButton) {
    previousButton = button;

    if (button == BTN_UP) {
      setFace(HAPPY);
      Serial.println("UP - HAPPY");
    }
    else if (button == BTN_RIGHT) {
      setFace(CONFUSED);
      Serial.println("RIGHT - CONFUSED");
    }
    else if (button == BTN_DOWN) {
      setFace(SAD);
      Serial.println("DOWN - SAD");
    }
    else if (button == BTN_LEFT) {
      setFace(ANGRY);
      Serial.println("LEFT - ANGRY");
    }
  }

  // MOVE SERVOS WHILE BUTTON HELD
  if (button != -1 &&
      now - lastMoveTime >= MOVE_INTERVAL) {

    lastMoveTime = now;

    if (button == BTN_UP)
      moveUp();

    else if (button == BTN_RIGHT)
      moveRight();

    else if (button == BTN_DOWN)
      moveDown();

    else if (button == BTN_LEFT)
      moveLeft();

    Serial.printf(
      "PAN=%d TILT=%d\n",
      panAngle, tiltAngle
    );
  }

  delay(10);
}
