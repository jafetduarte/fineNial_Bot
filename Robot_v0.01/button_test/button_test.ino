
const int UP = 4;
const int RIGHT = 5;
const int DOWN = 6;
const int LEFT = 7;

void setup() {
  Serial.begin(115200);

  pinMode(UP, INPUT_PULLDOWN);
  pinMode(RIGHT, INPUT_PULLDOWN);
  pinMode(DOWN, INPUT_PULLDOWN);
  pinMode(LEFT, INPUT_PULLDOWN);

  Serial.println("Button test ready!");
}

void loop() {
  if (digitalRead(UP) == HIGH)
    Serial.println("UP pressed");

  if (digitalRead(RIGHT) == HIGH)
    Serial.println("RIGHT pressed");

  if (digitalRead(DOWN) == HIGH)
    Serial.println("DOWN pressed");

  if (digitalRead(LEFT) == HIGH)
    Serial.println("LEFT pressed");

  delay(200);
}
