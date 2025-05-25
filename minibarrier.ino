#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// lcd obj
LiquidCrystal_I2C lcd(0x27,16,2);

const int greenLED = 13;
const int redLED = 12;
const int buttonPin = 2;

const int echoPin = 7;
const int trigPin = 6;

const int servoPin = 9;

const float DIST_PRAG = 15.0;
const int MAX_CARS = 10;

int carCounter = 0;
bool carDetected = false;
volatile bool buttonPressed = false;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, RISING);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(servoPin, OUTPUT);

  // fast pwm timer1 , top = ICR1
  TCCR1A = (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

  // set TOP (Period) to 20ms / 50Hz
  ICR1 = 40000;
  TCCR1A |= (1 << COM1A1);

  setServoAngle(90);

  lcd.setCursor(0, 0);
  lcd.print("Mini Barrier...");
  delay(1000);
}

void setServoAngle(int angle) {
  int pulseWidth = map(angle, 0, 180, 750, 2300);
  OCR1A = pulseWidth * 2;
}

void handleButtonPress() {
  buttonPressed = true;
}

void loop() {
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

  if (buttonPressed && carCounter > 0) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Car out...    ");
    setServoAngle(180);

    delay(2000);
    carCounter--;
    setServoAngle(90);
    buttonPressed = false;
  } else {
    if (distance < DIST_PRAG && carCounter < MAX_CARS) {
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Car in...      ");
      setServoAngle(180);

      delay(2000);
      carCounter++;
      setServoAngle(90);
      carDetected = true;
    } else {
      if (carCounter >= MAX_CARS) {
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("FULL! STOP       ");
        setServoAngle(90);
      } else {
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("Capacity: ");
        lcd.print(carCounter);
        lcd.print("/");
        lcd.print(MAX_CARS);
        lcd.print("   ");
        setServoAngle(90);
      }
    }
  }

  delay(300);
}