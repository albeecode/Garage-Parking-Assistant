/*
 Garage-Parking-Assistant
*/

#define HC1_trigPin 3
#define HC1_echoPin 4
#define HC1_grnLED 6
#define HC1_yelLED 7
#define HC1_redLED 8

unsigned long previousMillis = 0; // Set previous turn on time of LED during initialization
const long interval = 5000; // Set interval to turn off LED after no state change for 5 seconds
int ledState = LOW;

unsigned long grnCount = 0;
unsigned long yelCount = 0;
unsigned long redCount = 0;

void setup() {
  Serial.begin (9600);
  pinMode(HC1_trigPin, OUTPUT);
  pinMode(HC1_echoPin, INPUT);
  pinMode(HC1_redLED, OUTPUT);
  pinMode(HC1_yelLED, OUTPUT);
  pinMode(HC1_grnLED, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(HC1_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(HC1_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(HC1_trigPin, LOW);
  duration = pulseIn(HC1_echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance <= 200 || distance >= 0){
    if (distance < 15){
      ++redCount;
      yelCount = 0;
      grnCount = 0;
      digitalWrite(HC1_yelLED, LOW);
      digitalWrite(HC1_grnLED, LOW);
      ledControl(HC1_redLED, redCount);
    }
    else if (distance < 50){
      redCount = 0;
      ++yelCount;
      grnCount = 0;
      digitalWrite(HC1_redLED, LOW);
      digitalWrite(HC1_grnLED, LOW);
      ledControl(HC1_yelLED, yelCount);
    }
    else {
      redCount = 0;
      yelCount = 0;
      ++grnCount;
      digitalWrite(HC1_redLED, LOW);
      digitalWrite(HC1_yelLED, LOW);
      ledControl(HC1_grnLED, grnCount);
    }
    Serial.print(distance);
    Serial.println(" cm");
  }
  else {
    Serial.println("Out of range");
  }
  delay(250);
}

void ledControl(int ledPin, unsigned long ledCount) {
  unsigned long currentMillis = millis();

  if (ledCount == 1){
    // save the last time you turned on the LED
    previousMillis = currentMillis;
  }

  if (currentMillis - previousMillis >= interval) {
    ledState = LOW;
  }
  else {
    ledState = HIGH;
  }

  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, ledState);
}