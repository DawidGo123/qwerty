#include <Servo.h>

const int ledPin = 9;
const int servoPin = 10;
const int tempPin = A0;
const int buttonPin = 2;

Servo servo;

int servoAngle = 0;
bool servoEnabled = false;
unsigned long lastBlink = 0;
bool ledState = false;
unsigned long lastReport = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  servo.attach(servoPin);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    servoEnabled = !servoEnabled;
    delay(300); // debounce
  }

  unsigned long now = millis();

  if (now - lastBlink >= 500) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    lastBlink = now;
  }

  if (servoEnabled) {
    servoAngle = map(analogRead(tempPin), 0, 1023, 0, 180);
    servo.write(servoAngle);
  } else {
    servo.detach();
  }

  if (now - lastReport >= 1000) {
    float voltage = analogRead(tempPin) * (5.0 / 1023.0);
    float tempC = (voltage - 0.5) * 100; // LM35
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print(" C, Servo Enabled: ");
    Serial.println(servoEnabled ? "Yes" : "No");
    lastReport = now;
  }
}
