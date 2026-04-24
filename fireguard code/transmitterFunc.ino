// --- Inside transmitterFunc.ino ---

// You must install the RadioHead library for this to work!
#include <RH_ASK.h>
#include <SPI.h> // This is a built-in Arduino library that RadioHead requires to compile

// Initialize the RadioHead ASK (Amplitude Shift Keying) driver
// Parameter 1: 2000 = The speed of transmission (baud rate). 2000 is highly stable.
// Parameter 2: 11 = The Receive Pin. (We aren't receiving, but we have to declare a pin).
// Parameter 3: 12 = The Transmit Pin. **WIRE YOUR FS1000A DATA PIN HERE!**
RH_ASK rfDriver(2000, 11, 12);

void setupTransmitter() {
  // Start up the radio driver and check if it worked
  if (!rfDriver.init()) {
    Serial.println("RF Transmitter Initialization FAILED!");
  } else {
    Serial.println("RF Transmitter Ready on Pin 12.");
  }
}

void sendRFMessage(String dataString) {
  // 1. Convert the String
  // Radio radios cannot transmit standard Arduino "Strings" directly. 
  // We must convert your payload into an array of characters (bytes).
  const char *msg = dataString.c_str();
  
  // 2. Send the Data
  // Cast the characters into raw bytes (uint8_t) and tell the radio exactly how long the message is.
  rfDriver.send((uint8_t *)msg, strlen(msg));
  
  // 3. Wait
  // This physically blocks the Arduino for a few milliseconds to ensure the 
  // entire message blasts out of the antenna before the code moves on.
  rfDriver.waitPacketSent();
}