#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const int irSensorPin = D5;
const int buzzerPin = D6;
const int greenLEDPin = D7;
const int redLEDPin = D8;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Set the initial time manually (year, month, day, hour, minute, second)
  rtc.adjust(DateTime(2023, 1, 1, 12, 0, 0));

  pinMode(irSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

  Serial.println("Hardware test initialized");
}

void loop() {
  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinute = now.minute();
  Serial.print("Current Time: ");
  printDateTime(now);


  if (currentHour == 12 && currentMinute >= 0 && currentMinute <= 10) {
    // Medicine time
    int irSensorValue = digitalRead(irSensorPin);

    if (irSensorValue == HIGH) {
      // Hand interrupt detected, medicine taken
      Serial.println("Medicine is not taken");
       digitalWrite(buzzerPin, HIGH);  // Ring buzzer
      digitalWrite(redLEDPin, HIGH);   // Blink red LED
      delay(500);
      digitalWrite(redLEDPin, LOW);
      delay(500);
      digitalWrite(buzzerPin, LOW);
      
    } else {
      // Hand interrupt not detected, medicine not taken
      Serial.println("Medicine is taken");

      digitalWrite(greenLEDPin, HIGH);  // Blink green LED
      delay(500);
      digitalWrite(greenLEDPin, LOW);
      delay(500);
      Serial.println("Day is set to next day");
      delay(60000); //delay of 1 min for testing
    }
  }
}
void printDateTime(DateTime dt) {
  Serial.print(dt.year(), DEC);
  Serial.print('/');
  Serial.print(dt.month(), DEC);
  Serial.print('/');
  Serial.print(dt.day(), DEC);
  Serial.print(" ");
  Serial.print(dt.hour(), DEC);
  Serial.print(':');
  Serial.print(dt.minute(), DEC);
  Serial.print(':');
  Serial.print(dt.second(), DEC);
  Serial.println();
}
