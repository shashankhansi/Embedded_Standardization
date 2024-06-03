#include <Wire.h>

#define POSITION_NUM 2
#define ON LOW
#define OFF HIGH

const int SWITCH_PINS[] = {32, 23};

// Define relay pins
const int relayPins[8] = {4, 18, 19, 13, 27, 26, 25, 33};

// Variables to store received I2C data
volatile byte receivedType = 0;
volatile byte receivedValue = 0;
volatile bool newData = false;

int encoded_state = 0;
int currentSlaveId = 0; // Variable to store the current slave ID

// Function to read DIP switch value and set I2C address
void setI2CAddress() {
  encoded_state = 0;
  for (int i = 0; i < POSITION_NUM; i++) {
    int state = digitalRead(SWITCH_PINS[i]);

    if (state == ON)
      encoded_state |= 1 << (POSITION_NUM - i - 1);
  }

  // Print the DIP switch value
  Serial.print("DIP switch value: ");
  Serial.println(encoded_state);

  // Check if the DIP switch value is non-zero
  if (encoded_state != 0) {
    // Set the slave ID based on the DIP switch value
    currentSlaveId = encoded_state;

    // Reinitialize I2C with the new address
    Wire.begin(currentSlaveId);

    // Print the current slave ID
    Serial.print("Current Slave ID: ");
    Serial.println(currentSlaveId);
  } else {
    // If the DIP switch value is zero, print a message indicating the slave is inactive
    Serial.println("Slave inactive");
  }
}


void setup() {
  // Start serial communication at a baud rate of 115200.
  Serial.begin(115200);

  // Set the DIP switch pins as inputs with pull-up resistors enabled
  for (int i = 0; i < POSITION_NUM; i++)
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);

  // Set the initial I2C address based on the DIP switch position
  setI2CAddress();

  // Initialize I2C and register event handlers
  Wire.onReceive(receiveEvent);

  // Initialize relay pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // Make sure all relays are initially off
  }

  Serial.println("I2C Slave Initialized...");
}

void loop() {
  // Read the current DIP switch value and update the I2C address if necessary
  setI2CAddress();

  // Process new I2C data if available
  if (newData) {
    newData = false;

    // Print the received data and the current slave ID to the Serial Monitor
    Serial.print("Received from Slave ");
    Serial.print(currentSlaveId);
    Serial.print(": Type: ");
    Serial.print(receivedType);
    Serial.print(", Value: ");
    Serial.println(receivedValue);

    // Process the received type and value
    if (receivedType == 0) {
      // Control the relay corresponding to the received value
      if (receivedValue >= 1 && receivedValue <= 8) {
        int relayIndex = receivedValue - 1; // Adjust to array index
        digitalWrite(relayPins[relayIndex], HIGH); // Turn on the relay
        Serial.print("Turned ON relay ");
        Serial.println(receivedValue);
      } else {
        Serial.println("Invalid relay number");
      }
    } else if (receivedType == 1) {
      // Control the relay corresponding to the received value
      if (receivedValue >= 1 && receivedValue <= 8) {
        int relayIndex = receivedValue - 1; // Adjust to array index
        digitalWrite(relayPins[relayIndex], LOW); // Turn off the relay
        Serial.print("Turned OFF relay ");
        Serial.println(receivedValue);
      } else {
        Serial.println("Invalid relay number");
      }
    }
  }

  // Add a small delay to avoid excessive I2C traffic and to debounce DIP switch reading
  delay(1000);
}

// I2C receive event handler
void receiveEvent(int howMany) {
  // Check if the slave is active (currentSlaveId is non-zero)
  if (currentSlaveId != 0 && howMany >= 2) {
    receivedType = Wire.read();  // Read the type byte
    receivedValue = Wire.read(); // Read the value byte
    newData = true;              // Set flag to indicate new data
  }
}
