#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// lcd obj
LiquidCrystal_I2C lcd(0x27,16,2);

const int greenLED = 13;
const int redLED = 12;
const int buttonPin = 11;

const int echoPin = 10;
const int trigPin = 9;

const float DIST_PRAG = 15.0;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Sistem pornit...");
  delay(1000);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  float distance = duration * 0.034 / 2;

  // Afișăm pe LCD
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm   ");
  delay(100);

  lcd.setCursor(0, 1);

  if (buttonState == HIGH) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    lcd.print("Mod: MANUAL      ");
  } else {
    // Automat, pe baza senzorului
    if (distance < DIST_PRAG) {
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);
      lcd.print("Mod: AUTO - OK   ");
    } else {
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);
      lcd.print("Mod: AUTO - STOP ");
    }
  }

  delay(100);
}