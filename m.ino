#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <SPI.h>

// Define pins for joystick
const int joyXPin = 26; // ADC0 (GP26)
const int joyYPin = 27; // ADC1 (GP27)
const int joyButtonPin = 2;

// Define pins for LED matrix
const int csPin = 5; // Chip Select
const int clkPin = 18; // Clock
const int dinPin = 19; // Data in

// Define pins for buzzers
const int pointBuzzerPin = 3;
const int effectBuzzerPin = 4;

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

int x = 3; // Initial X position of the dot
int y = 3; // Initial Y position of the dot
int score = 0;
bool gameRunning = true;

void setup() {
  pinMode(joyXPin, INPUT);
  pinMode(joyYPin, INPUT);
  pinMode(joyButtonPin, INPUT_PULLUP);

  pinMode(pointBuzzerPin, OUTPUT);
  pinMode(effectBuzzerPin, OUTPUT);

  SPI.begin();
  matrix.begin(csPin, clkPin, dinPin);
  matrix.setRotation(2); // Adjust if necessary
  matrix.clear();
  matrix.writeDisplay();
}

void loop() {
  if (digitalRead(joyButtonPin) == LOW) {
    gameRunning = false;
    showScore();
  }

  if (gameRunning) {
    int joyX = analogRead(joyXPin);
    int joyY = analogRead(joyYPin);

    // Update dot position based on joystick input
    if (joyX < 400 && x > 0) {
      x--;
      tone(effectBuzzerPin, 500, 100); // Play sound effect
    } else if (joyX > 600 && x < 7) {
      x++;
      tone(effectBuzzerPin, 500, 100); // Play sound effect
    }

    if (joyY < 400 && y > 0) {
      y--;
      tone(effectBuzzerPin, 500, 100); // Play sound effect
    } else if (joyY > 600 && y < 7) {
      y++;
      tone(effectBuzzerPin, 500, 100); // Play sound effect
    }

    // Clear the matrix and draw the new dot position
    matrix.clear();
    matrix.drawPixel(x, y, LED_ON);
    matrix.writeDisplay();

    delay(100); // Small delay to slow down the movement
  }
}

void showScore() {
  matrix.clear();
  for (int i = 0; i < 5; i++) {
    matrix.setCursor(1, 0);
    matrix.print(score);
    matrix.writeDisplay();
    delay(500);
    matrix.clear();
    matrix.writeDisplay();
    delay(500);
  }
}
