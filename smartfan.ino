#include <DHT.h>

// Temperature sensor
#define DHTPIN 2 
#define DHTTYPE DHT11

// Ultrasonic sensor
#define TRIGPIN 3
#define ECHOPIN 4 

// Motor for the fan
#define FANSPEEDPIN 9
#define FANENABLEPIN 10

// LEDs
#define LED_GREEN 3
#define LED_YELLOW 4
#define LED_BLUE 5
#define LED_RED 6

// Button for adjusting speed
#define BUTTONPIN 7

// DHT (temperature) sensor instance
DHT dht(DHTPIN, DHTTYPE);

int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int speedLevels[4] = {0, 85, 170, 255};
int currentSpeedLevel = 0;

void setup() {
  pinMode(FANSPEEDPIN, OUTPUT);
  pinMode(FANENABLEPIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);
  dht.begin();

  Serial.begin(9600);
}

void loop() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }
  Serial.println("Current temp: ");
  Serial.println(temp);

  /*
  bool isClose = isObjectClose();
  if (temp > 22.5 && isClose) {
    digitalWrite(FANENABLEPIN, HIGH);
    analogWrite(FANSPEEDPIN, speedLevels[currentSpeedLevel]);
  } else {
    digitalWrite(FANENABLEPIN, LOW);
  }
  int reading = digitalRead(BUTTONPIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {
        currentSpeedLevel++;
      if (currentSpeedLevel > 3) {
        currentSpeedLevel = 0;
      }
      analogWrite(FANSPEEDPIN, speedLevels[currentSpeedLevel]);
      updateLEDS(currentSpeedLevel);
    }
  }
  lastButtonState = reading;
  */

  // Small delay to avoid reading noise
  delay(10);
}

bool isObjectClose() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  long duration = pulseIn(ECHOPIN, HIGH);
  float distance = (duration / 2) * 0.0343;

  return distance < 20; // Returns true if object is closer than 20 cm
}

void updateLEDS(int level) {
  // Turn off all LEDs
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);

  // Turn on a specific LED based on the current speed level
  if (level == 1) {
    digitalWrite(LED_GREEN, HIGH);
  } else if (level == 2) {
    digitalWrite(LED_YELLOW, HIGH);
  } else if (level == 3) {
    digitalWrite(LED_BLUE, HIGH);
  } else if (level == 4) {
    digitalWrite(LED_RED, HIGH);
  }
}