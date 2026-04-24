// --- Inside tempFunc.ino ---

#include "DHT.h"

#define DHTPIN 2     
#define DHTTYPE DHT11   

// Initialize the sensor
DHT dht(DHTPIN, DHTTYPE);

void tempsetup() {
  // We remove Serial.begin here because main.ino already does it!
  dht.begin();
}

// Our new function that returns a float!
float getTemperature() {
  // Read temperature as Fahrenheit (true means Fahrenheit)
  float tempF = dht.readTemperature(true);

  // Check if the read failed (isnan means "Is Not A Number")
  if (isnan(tempF)) {
    // If the sensor gets disconnected or fails, return an obvious fake number.
    // This way, your UI will see "-999.0" and know the sensor is broken!
    return -999.0; 
  }

  // Hand the valid temperature back to main.ino
  return tempF;
}