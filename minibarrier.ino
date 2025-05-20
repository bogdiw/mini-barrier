#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// lcd obj
LiquidCrystal_I2C lcd(0x27,16,2);

const int greenLED = 13;
const int redLED = 12;
const int buttonPin = 11;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("AJUTOR");

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Buton: APASAT  ");
  } else {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Buton: NEAPASAT");
  }

  delay(100);
}