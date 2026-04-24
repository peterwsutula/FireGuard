// --- MAIN.INO ---
// Remember: No #include needed for your other .ino files!

// Variables for our non-blocking timer
unsigned long previousMillis = 0;
const long interval = 2000; // How often to send data in milliseconds (1000 = 1 second)

void setup() {
  // Start the Serial monitor for testing on your computer
  Serial.begin(9600);
  
  // Call the setup functions you wrote in your other tabs
  // (Assuming you made these functions to initialize the pins)
  motionsetup();
  qualsetup();
  tempsetup();
  setupTransmitter();
  
  Serial.println("Firefighter Monitor Initialized.");
}

void loop() {
  // 1. CONSTANT BACKGROUND TASKS
  // EX: If your motion sensor needs to be checked constantly to update its 20-second timer, 
  // put that update function here so it runs thousands of times a second.
  // updateMotionTracker(); 
  motionloop();
  qualloop();

  // 2. TIMED TRANSMISSION
  // Check the "stopwatch" to see if 1 second has passed
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // Reset the stopwatch
    previousMillis = currentMillis;

    // --- A. GATHER DATA ---
    // Pull the latest data from the functions in your other .ino files
    float currentTemp = getTemperature();
    bool isMoving = checkMotion(); 
    float co2 = getCO2();
    float nh4 = getNH4();
    float nh3 = getNH3();
    float toluene = getToluene();

// --- B. PACKAGE DATA ---
    // Create an empty string
    String payload = "";
    
    // Tell the Arduino to set aside 60 bytes of memory right now so it doesn't crash later
    payload.reserve(60); 

    // Add each piece of data one step at a time
    payload += String(currentTemp);
    payload += ",";
    payload += String(isMoving);
    payload += ",";
    payload += String(co2);
    payload += ",";
    payload += String(nh4);
    payload += ",";
    payload += String(nh3);
    payload += ",";
    payload += String(toluene);

    // --- C. TRANSMIT DATA ---
    // Hand the packaged string to your RF transmitter function
    sendRFMessage(payload);

    // Print to the Serial Monitor so you can verify it works while plugged into USB
    Serial.print("Data Sent over RF: ");
    Serial.println(payload);
  }
}