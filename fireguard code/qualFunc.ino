#include <MQUnifiedsensor.h>

// Definitions
#define Board           ("Arduino UNO")
#define Pin             (A1) // Analog input 1
#define Type            ("MQ-135") 
#define Voltage_Resolution      (5)
#define ADC_Bit_Resolution      (10) // For Arduino UNO
#define RatioMQ135CleanAir      (3.6) // RS / R0 ratio for clean air

MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

// --- NEW GLOBAL VARIABLES ---
// These will store the latest readings safely
float currentCO2 = 0.0;
float currentNH4 = 0.0;
float currentToluene = 0.0;
float currentNH3 = 0.0;

void qualsetup() {
  // Serial.begin removed because main.ino handles it!
  MQ135.setRegressionMethod(1); 
  MQ135.init(); 

  Serial.print("Calibrating MQ-135... Please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++) {
    MQ135.update(); 
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
  }
  MQ135.setR0(calcR0/10);
  Serial.println(" Done!");
}

void qualloop() {
  MQ135.update(); // Read the raw sensor data

  // Calculate and store each gas into our global variables
  MQ135.setA(110.47); MQ135.setB(-2.862); 
  currentCO2 = MQ135.readSensor(); 

  MQ135.setA(10.231); MQ135.setB(-4.027); 
  currentNH4 = MQ135.readSensor(); 

  MQ135.setA(44.947); MQ135.setB(-3.445); 
  currentToluene = MQ135.readSensor(); 

  MQ135.setA(102.2 ); MQ135.setB(-2.473); 
  currentNH3 = MQ135.readSensor(); 

  // REMOVED delay(1000) and Serial.print so the main loop never freezes!
}

// --- NEW GETTER FUNCTIONS ---
// These hand the global variables back to main.ino when asked

float getCO2() {
  return currentCO2;
}

float getNH4() {
  return currentNH4;
}

float getToluene() {
  return currentToluene;
}

float getNH3() {
  return currentNH3;
}