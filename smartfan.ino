#include <DHT.h>

// Temperature sensor
#define DHTPIN 2 // Data PIN of DHT11 (temp sensor)
#define DHTTYPE DHT11 // Type of temp sensor

// Ultrasonic sensor
#define TRIGPIN 11 // Trigger to start distance measurement
#define ECHOPIN 12 // ECHOPIN is used to read the distance


// Motor for the fan
#define FANSPEEDPIN 9   // Controlls the speed of the fan
#define FANENABLEPIN 10 //Enables/disables the motor that spins the fan

// LEDs
#define LED_GREEN 3
#define LED_YELLOW 4
#define LED_BLUE 5
#define LED_RED 6

// Button for adjusting speed
#define BUTTONPIN 7

// DHT (temperature) sensor instance
DHT dht(DHTPIN, DHTTYPE);

int lastButtonState = LOW; // keeps track of previous state (high or low) of the button. Tells the if the button is being pressed or not.

int speedLevels[4] = {0, 100, 170, 255};
int currentSpeedLevel = 2;

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

void loop() { // arduino calls the loop infinitely
  float temp = dht.readTemperature();
  if (isnan(temp)) { // checks if it can read from the sensor (temperature sensor)
    Serial.println("Failed to read from DHT sensor");
    return;
  }
  // prints the current temperature
  Serial.println("Current temp: ");
  Serial.println(temp);
  // checks if something is near the ultrasonic sensor
  bool isClose = isObjectClose();

  // it checks if the temp is over 24c and there is an object nearby. If it meets the requirements, it starts the fan.
  if (temp > 24 && isClose) {
    digitalWrite(FANENABLEPIN, HIGH);
    analogWrite(FANSPEEDPIN, speedLevels[currentSpeedLevel]);
  } else {
    digitalWrite(FANENABLEPIN, LOW);
  }

  // When the button is pressed, the code cycles through the fan speeds and LEDs change.
  int reading = digitalRead(BUTTONPIN);
  if ((lastButtonState == LOW) && (reading == HIGH)) {
    currentSpeedLevel++;
    if (currentSpeedLevel > 3) {
      currentSpeedLevel = 0;
    }
    analogWrite(FANSPEEDPIN, speedLevels[currentSpeedLevel]);
    updateLEDS(currentSpeedLevel);
  }

  lastButtonState = reading;

  // Small delay to avoid reading noise
  delay(2);
}
// when the distance is less than 20cm, it returns true.
bool isObjectClose() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  long duration = pulseIn(ECHOPIN, HIGH);
  float distance = (duration / 2) * 0.0343;

  Serial.println("Current Distance:");
  Serial.println(distance);

  return distance < 20; // Returns true if object is closer than 20 cm
}


void updateLEDS(int level) { // updates the LEDs which corresponds to the speed.
  // Turn off all LEDs
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);

  // Turn on a specific LED based on the current speed level
  if (level == 0) {
    digitalWrite(LED_GREEN, HIGH);
  } else if (level == 1) {
    digitalWrite(LED_YELLOW, HIGH);
  } else if (level == 2) {
    digitalWrite(LED_BLUE, HIGH);
  } else if (level == 3) {
    digitalWrite(LED_RED, HIGH);
  }
}
