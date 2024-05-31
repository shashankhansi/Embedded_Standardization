
#include <Wire.h>

// Define I2C address for the slave
#define Slave1 0x01

// Define relay pins
const int relayPins[8] = {4,18,19,13,27,26,25,33};

// Variables to store received I2C data
volatile byte receivedType = 0;
volatile byte receivedValue = 0;
volatile bool newData = false;

void setup() {
  // Start serial communication at a baud rate of 115200.
  Serial.begin(115200);

  // Initialize I2C and register event handler
  Wire.begin(Slave1);
  Wire.onReceive(receiveEvent);

  // Initialize relay pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // Make sure all relays are initially off
  }

  Serial.println("I2C Slave Initialized...");
}

void loop() {
  // Process new I2C data if available
  if (newData) {
    newData = false;

    // Print the received data to the Serial Monitor
    Serial.print("Received Type: ");
    Serial.println(receivedType);
    Serial.print("Received Value: ");
    Serial.println(receivedValue);

    // Process the received type and value
    if (receivedType == 0) {
      // Control the relay corresponding to the received value
      if (receivedValue >= 1 && receivedValue <= 8) {
        int relayIndex = receivedValue - 1; // Adjust to array index
        digitalWrite(relayPins[relayIndex], HIGH); // Turn on the relay
        Serial.print("Turned OFF relay ");
        Serial.println(receivedValue);
      } else {
        Serial.println("Invalid relay number");
      }
    } else if (receivedType == 1) {
      // Control the relay corresponding to the received value
      if (receivedValue >= 1 && receivedValue <= 8) {
        int relayIndex = receivedValue - 1; // Adjust to array index
        digitalWrite(relayPins[relayIndex], LOW); // Turn off the relay
        Serial.print("Turned ON relay ");
        Serial.println(receivedValue);
      } else {
        Serial.println("Invalid relay number");
      }
    }
  }
}

// I2C receive event handler
void receiveEvent(int howMany) {
  if (howMany >= 2) {
    receivedType = Wire.read();  // Read the type byte
    receivedValue = Wire.read(); // Read the value byte
    newData = true;              // Set flag to indicate new data
  }
}




// Below code is only for when we are using another slave(SLAVE2)
//after compl

// #include <Wire.h>

// // Define I2C address for the slave
// #define Slave2 0x02

// // Define relay pins
// const int relayPins[8] = {4,18,19,13,27,26,25,33};

// // Variables to store received I2C data
// volatile byte receivedType = 0;
// volatile byte receivedValue = 0;
// volatile bool newData = false;

// void setup() {
//   // Start serial communication at a baud rate of 115200.
//   Serial.begin(115200);

//   // Initialize I2C and register event handler
//   Wire.begin(Slave2);
//   Wire.onReceive(receiveEvent);

//   // Initialize relay pins as outputs
//   for (int i = 0; i < 8; i++) {
//     pinMode(relayPins[i], OUTPUT);
//     digitalWrite(relayPins[i], HIGH); // Make sure all relays are initially off
//   }

//   Serial.println("I2C Slave Initialized...");
// }

// void loop() {
//   // Process new I2C data if available
//   if (newData) {
//     newData = false;

//     // Print the received data to the Serial Monitor
//     Serial.print("Received Type: ");
//     Serial.println(receivedType);
//     Serial.print("Received Value: ");
//     Serial.println(receivedValue);

//     // Process the received type and value
//     if (receivedType == 0) {
//       // Control the relay corresponding to the received value
//       if (receivedValue >= 9 && receivedValue <= 16) {
//         int relayIndex = receivedValue - 9; // Adjust to array index
//         digitalWrite(relayPins[relayIndex], HIGH); // Turn on the relay
//         Serial.print("Turned OFF relay ");
//         Serial.println(receivedValue - 8);
//       } else {
//         Serial.println("Invalid relay number");
//       }
//     } else if (receivedType == 1) {
//       // Control the relay corresponding to the received value
//       if (receivedValue >= 9 && receivedValue <= 16) {
//         int relayIndex = receivedValue - 9; // Adjust to array index
//         digitalWrite(relayPins[relayIndex], LOW); // Turn off the relay
//         Serial.print("Turned ON relay ");
//         Serial.println(receivedValue - 8);
//       } else {
//         Serial.println("Invalid relay number");
//       }
//     }
//   }
// }

// // I2C receive event handler
// void receiveEvent(int howMany) {
//   if (howMany >= 2) {
//     receivedType = Wire.read();  // Read the type byte
//     receivedValue = Wire.read(); // Read the value byte
//     newData = true;              // Set flag to indicate new data
//   }
// }
