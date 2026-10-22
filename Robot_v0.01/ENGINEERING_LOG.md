October 22, 2026 — Hardware Testing

Test 1: Servo Motors
Program: servo_test.ino
Result: SUCESS
Observations:works both modes, turning angles are short
Problems encountered: servo internally broken
Changes made:replaced toa different servo


Next steps:
Test 2: LCD Display
Program: face_test.ino
Result: SUCESS
Observations: tested, screen is a little glitchy(could eb connections) 
Problems encountered: small crack
Changes made: N/A


Test 3: Directional Buttons
Program: button_test.ino
Result: SUCCESS
Observations: buttions work
Problems encountered: n/a
Changes made:n/a


Test 4: Complete Robot
Program: Robot_v0.01.ino
Result: SUCCESS
Observations: had to switch linraries
Problems encountered: would get stuck in an infinite loop for servo movement
Changes made: changed libraries used


SUMMARY:
First successful integrated robot test

Combined the ESP32-S3, ST7735 LCD, four directional buttons, and two servos. The original ESP32Servo library caused the program to freeze during servo attachment. Replaced the library with direct LEDC PWM control at 50 Hz, resolving the issue. The integrated system now functions.