#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Variables for motion detection
bool isMoving = true;
unsigned long lastMotionTime = 0;
const unsigned long idleThreshold = 20000; // 20 seconds in milliseconds

// Sensitivity threshold - adjust this if it's too sensitive or not sensitive enough
const float motionThreshold = 0.75; 

// Storage for previous readings to compare
float lastX, lastY, lastZ;

void motionsetup() {
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }

  Serial.println("MPU6050 Found! Calibrating baseline...");
  
  // Get initial readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  lastX = a.acceleration.x;
  lastY = a.acceleration.y;
  lastZ = a.acceleration.z;
  
  lastMotionTime = millis();
}

void motionloop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate the change (delta) in acceleration
  float deltaX = abs(a.acceleration.x - lastX);
  float deltaY = abs(a.acceleration.y - lastY);
  float deltaZ = abs(a.acceleration.z - lastZ);

  // Check if any axis moved more than the threshold
  if (deltaX > motionThreshold || deltaY > motionThreshold || deltaZ > motionThreshold) {
    isMoving = true;
    lastMotionTime = millis(); // Reset the timer because we moved
  } else {
    // If we haven't moved, check how long it's been
    if (millis() - lastMotionTime > idleThreshold) {
      isMoving = false;
    }
  }

  // Update "last" values for the next loop
  lastX = a.acceleration.x;
  lastY = a.acceleration.y;
  lastZ = a.acceleration.z;
}

bool checkMotion(){
  bool check = isMoving;
  return check;
}