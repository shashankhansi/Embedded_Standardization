#include <Wire.h>

#define Slave1 0x01  // Adjusted to a 7-bit I2C address
#define Slave2 0x02  // Adjusted to a 7-bit I2C address

#define Relay1  1
#define Relay2  2
#define Relay3  3
#define Relay4  4
#define Relay5  5
#define Relay6  6
#define Relay7  7
#define Relay8  8
#define Relay9  9
#define Relay10  10
#define Relay11  11
#define Relay12  12
#define Relay13  13
#define Relay14  14
#define Relay15  15
#define Relay16  16

void setup() {
  // Start serial communication at a baud rate of 115200.
  Serial.begin(115200);
  // Initialize I2C communication.
  Wire.begin();
}

void sendDataToSlave(uint8_t slaveId, byte type, byte val) {
  // This function sends data to the slave over I2C.
  Wire.beginTransmission(slaveId); // Begin I2C transmission to the slave with the given ID.
  Wire.write(type);                // Send the type to the slave.
  Wire.write(val);                 // Send the val to the slave.
  Wire.endTransmission();          // End the transmission.
  
  // Print to the Serial Monitor for debugging purposes.
  Serial.print("Sending data to Slave ");
  Serial.print(slaveId, HEX);
  Serial.print(": Type ");
  Serial.print(type);
  Serial.print(", Value ");
  Serial.println(val);
}

void loop() {
  // Check if data is available to read from the serial port.
  if (Serial.available() > 0) {
    // Read the incoming data as a string.
    String data = Serial.readStringUntil('\n');

    // Assuming the data string is in the format "<type><val1>".
    // For example, "01" or "12".
    if (data.length() >= 2) {
      int type = data.charAt(0) - '0';  // First character converted to integer.
      int val1 = data.substring(1).toInt(); // Remaining characters converted to integer.

      // Check if type is 0 or 1 and val1 is between 1 and 8 or 9 and 16.
      if ((type == 0 || type == 1) && val1 >= 1 && val1 <= 8) {
        sendDataToSlave(Slave1, type, val1);
      } else if ((type == 0 || type == 1) && val1 >= 9 && val1 <= 16) {
        sendDataToSlave(Slave2, type, val1);
      } else {
        Serial.println("Invalid type or relay number");
      }

      // Print values to the Serial Monitor for debugging.
      Serial.print("Type: ");
      Serial.println(type);
      Serial.print("Val1: ");
      Serial.println(val1);
    } else {
      Serial.println("Invalid data format");
    }
  }
}
