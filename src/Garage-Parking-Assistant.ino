#include <Arduino.h>

/*
 Garage-Parking-Assistant
 This arduino sketch will work with 2 HC-SR04 distance sensors to read distance in cm.
 Each of the 2 HC-SR04 distance sensors will have a Red, Yellow, and Green LED attached for distance caluclation.
 If the distance is between 0 and 14 cm the red LED for the given sensor will light up.
 If the distance is between 15 and 49 cm the yellow LED for the given sensor will light up.
 If the distance is between 50 and 200 cm the green LED for the given sensor will light up.
*/

// Define Pins for Sensor 1 and Sensor 1 LEDs
#define HC1_trigPin 8
#define HC1_echoPin 9
#define HC1_grnLED 10
#define HC1_yelLED 11
#define HC1_redLED 12

// Define Pins for Sensor 2 and Sensor 2 LEDs
#define HC2_trigPin 3
#define HC2_echoPin 4
#define HC2_grnLED 5
#define HC2_yelLED 6
#define HC2_redLED 7

unsigned long HC1_previousMillis = 0; // Set previous turn on time of LED during initialization for Sensor 1
unsigned long HC2_previousMillis = 0; // Set previous turn on time of LED during initialization for Sensor 2
const long interval = 5000; // Set interval to turn off LED after no state change for 5 seconds
int ledState = LOW;

// Set variables for counts on sensor 1, used for turning off LED after 5 seconds of inactivity
unsigned long HC1_grnCount = 0;
unsigned long HC1_yelCount = 0;
unsigned long HC1_redCount = 0;

// Set variables for counts on sensor 2, used for turning off LED after 5 seconds of inactivity
unsigned long HC2_grnCount = 0;
unsigned long HC2_yelCount = 0;
unsigned long HC2_redCount = 0;

void setup() {
  Serial.begin (9600);
  pinMode(HC1_trigPin, OUTPUT);
  pinMode(HC1_echoPin, INPUT);
  pinMode(HC1_redLED, OUTPUT);
  pinMode(HC1_yelLED, OUTPUT);
  pinMode(HC1_grnLED, OUTPUT);
  pinMode(HC2_trigPin, OUTPUT);
  pinMode(HC2_echoPin, INPUT);
  pinMode(HC2_redLED, OUTPUT);
  pinMode(HC2_yelLED, OUTPUT);
  pinMode(HC2_grnLED, OUTPUT);
}

void ledControl(int sensornum, int ledPin, unsigned long &ledCount) {
  unsigned long currentMillis = millis();

  if (sensornum == 1) {
    if (ledCount == 1){
      // save the last time you turned on the LED
      HC1_previousMillis = currentMillis;
    }

    if (currentMillis - HC1_previousMillis >= interval) {
      ledState = LOW;
    }
    else {
      ledState = HIGH;
    }
  }
  else if (sensornum == 2) {
    if (ledCount == 1){
      // save the last time you turned on the LED
      HC2_previousMillis = currentMillis;
    }
  
    if (currentMillis - HC2_previousMillis >= interval) {
      ledState = LOW;
    }
    else {
      ledState = HIGH;
    }
  }

  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, ledState);
}

void queryDistance(int sensornum, int trigpin, int echopin, int redLEDpin, int yelLEDpin, int grnLEDpin, unsigned long &redCount, unsigned long &yelCount, unsigned long &grnCount) {
  long duration, distance;
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance <= 200 && distance >= 0){
    if (distance < 15) {
      ++redCount;
      yelCount = 0;
      grnCount = 0;
      digitalWrite(yelLEDpin, LOW);
      digitalWrite(grnLEDpin, LOW);
      ledControl(sensornum, redLEDpin, redCount);
    }
    else if (distance < 50) {
      redCount = 0;
      ++yelCount;
      grnCount = 0;
      digitalWrite(redLEDpin, LOW);
      digitalWrite(grnLEDpin, LOW);
      ledControl(sensornum, yelLEDpin, yelCount);
    }
    else {
      redCount = 0;
      yelCount = 0;
      ++grnCount;
      digitalWrite(redLEDpin, LOW);
      digitalWrite(yelLEDpin, LOW);
      ledControl(sensornum, grnLEDpin, grnCount);
    }
    Serial.print(distance);
    Serial.println(" cm");
  }
  else {
    // Distance out of range, reset LED Counters and set all LED's off
    redCount = 0;
    yelCount = 0;
    grnCount = 0;

    digitalWrite(redLEDpin, LOW);
    digitalWrite(yelLEDpin, LOW);
    digitalWrite(grnLEDpin, LOW);

    Serial.println("Out of range");
  }
  delay(250);
}

void loop() {
  queryDistance(1, HC1_trigPin, HC1_echoPin, HC1_redLED, HC1_yelLED, HC1_grnLED, HC1_redCount, HC1_yelCount, HC1_grnCount);
  queryDistance(2, HC2_trigPin, HC2_echoPin, HC2_redLED, HC2_yelLED, HC2_grnLED, HC2_redCount, HC2_yelCount, HC2_grnCount);
}