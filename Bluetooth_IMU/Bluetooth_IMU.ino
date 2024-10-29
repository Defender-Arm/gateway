#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); // I2C address 0x28 for BNO055
BluetoothSerial SerialBT;  // Instantiate Bluetooth serial object

void setup() {
    Serial.begin(115200);      // Start Serial for monitoring
    SerialBT.begin("ESP32_BT"); // Start Bluetooth with a name (e.g., "ESP32_BT")
    Serial.println("Bluetooth started! Connect to 'ESP32_BT'");

    if (!bno.begin()) {
        Serial.println("Could not find a valid BNO055 sensor, check wiring!");
        while (1);
    }

    delay(1000); // Allow time for sensor to initialize
    bno.setExtCrystalUse(true); // Use external crystal for accuracy
}

void loop() {
    // Sensor data retrieval
    sensors_event_t linearAccelData;
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    // Send linear acceleration data only, formated with each value separated by ','
    SerialBT.print(linearAccelData.acceleration.x);
    SerialBT.print(",");
    SerialBT.print(linearAccelData.acceleration.y);
    SerialBT.print(",");
    SerialBT.println(linearAccelData.acceleration.z);

    delay(500); // Send data every 500ms
}

    // // Sensor data retrieval
    // sensors_event_t orientationData, linearAccelData;
    // bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    // bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    // // Send data over Bluetooth
    // SerialBT.print("Orientation (Degrees): ");
    // SerialBT.print("X: "); SerialBT.print(orientationData.orientation.x);
    // SerialBT.print(", Y: "); SerialBT.print(orientationData.orientation.y);
    // SerialBT.print(", Z: "); SerialBT.println(orientationData.orientation.z);

    // SerialBT.print("Linear Acceleration (m/s^2): ");
    // SerialBT.print("X: "); SerialBT.print(linearAccelData.acceleration.x);
    // SerialBT.print(", Y: "); SerialBT.print(linearAccelData.acceleration.y);
    // SerialBT.print(", Z: "); SerialBT.println(linearAccelData.acceleration.z);

    // // Check if data is available to read
    // if (SerialBT.available()) {
    //     String received = SerialBT.readString();  // Read incoming data
    //     Serial.print("Received: ");
    //     Serial.println(received);  // Print received data to Serial Monitor
    // }

    // delay(500); // Wait 0.5 seconds before repeating
// }
