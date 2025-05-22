#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// lcd obj
LiquidCrystal_I2C lcd(0x27,16,2);

const int greenLED = 13;
const int redLED = 12;
const int buttonPin = 11;

const int echoPin = 10;
const int trigPin = 9;

const int servoPin = 7;
Servo bariera;

const float DIST_PRAG = 15.0;
const int MAX_CARS = 10;

int contorMasini = 0;
bool masinaDetectata = false;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  bariera.attach(servoPin);
  bariera.write(0);

  lcd.setCursor(0, 0);
  lcd.print("Mini Barrier...");
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

  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm   ");
  delay(100);

  lcd.setCursor(0, 1);

  if (buttonState == HIGH && contorMasini > 0) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    bariera.write(90);
    lcd.setCursor(0, 1);
    lcd.print("Car out...    ");

    delay(2000);
    contorMasini--;
    bariera.write(0);
  } else {
    if (distance < DIST_PRAG && contorMasini < MAX_CARS) {
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);
      bariera.write(90);
      lcd.setCursor(0, 1);
      lcd.print("Car in...      ");

      delay(2000);
      contorMasini++;
      bariera.write(0);
      masinaDetectata = true;
    } else {
      if (contorMasini >= MAX_CARS) {
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        bariera.write(0);
        lcd.setCursor(0, 1);
        lcd.print("FULL! STOP       ");
      } else {
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        bariera.write(0);
        lcd.setCursor(0, 1);
        lcd.print("Capacity: ");
        lcd.print(contorMasini);
        lcd.print("/");
        lcd.print(MAX_CARS);
        lcd.print("   ");
      }
    }
  }

  delay(300);
}