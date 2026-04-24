// --- RECEIVERMAIN.INO ---

// You must install the RadioHead library on this computer/board too!
#include <RH_ASK.h>
#include <SPI.h> 

// Initialize the RadioHead ASK driver
// Parameter 1: 2000 = The baud rate (MUST match the transmitter exactly)
// Parameter 2: 11 = The Receive Pin (Where your MX-RM-5V DATA pin is plugged in)
// Parameter 3: 12 = The Transmit Pin (We aren't transmitting, but the library requires a pin number here)
RH_ASK rfDriver(2000, 11, 12);

void setup() {
  Serial.begin(9600);
  Serial.println("Booting up Fireguard Receiver...");
  
  if (!rfDriver.init()) {
    Serial.println("RF Receiver Initialization FAILED!");
  } else {
    Serial.println("RF Receiver Ready and listening on Pin 11...");
  }
}

void loop() {
  // 1. Create a "bucket" to catch the incoming bytes. 
  // We reserved 60 bytes on the transmitter, so we make a 60-byte bucket here.
  uint8_t buf[60];
  uint8_t buflen = sizeof(buf);

  // 2. Listen for a clean message
  // The recv() function listens to the static, verifies the checksum, 
  // and returns 'true' ONLY if a perfect, uncorrupted message arrives.
  if (rfDriver.recv(buf, &buflen)) {
    
    // --- THE NULL TERMINATOR TRAP ---
    // RadioHead just sends raw bytes, not text. If we don't tell the Arduino exactly 
    // where the message stops, it will keep reading empty memory and print garbage characters.
    // We fix this by manually adding a "0" (null terminator) to the very end of the received length.
    buf[buflen] = '\0'; 

    // 3. Convert the raw bytes back into a readable String
    String receivedData = String((char*)buf);
    
    // 4. Print it out!
    Serial.print("Data Received: ");
    Serial.println(receivedData);
  }
}