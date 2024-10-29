#include "BluetoothSerial.h"

BluetoothSerial SerialBT;  // Instantiate Bluetooth serial object

void setup() {
    Serial.begin(115200);      // Start Serial for monitoring
    SerialBT.begin("ESP32_BT"); // Start Bluetooth with a name (e.g., "ESP32_BT")
    Serial.println("Bluetooth started! Connect to 'ESP32_BT'");
}

void loop() {
    // Send "Hello World" every 2 seconds
    SerialBT.println("Hello World");
    delay(2000);

    // Check if data is available to read
    if (SerialBT.available()) {
        String received = SerialBT.readString();  // Read incoming data
        Serial.print("Received: ");
        Serial.println(received);  // Print received data to Serial Monitor
    }
}
