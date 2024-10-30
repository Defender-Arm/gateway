#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
BluetoothSerial SerialBT;

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32_BT");
    Serial.println("Bluetooth started! Connect to 'ESP32_BT'");

    if (!bno.begin()) {
        Serial.println("Could not find a valid BNO055 sensor, check wiring!");
        while (1);
    }

    delay(1000);
    bno.setExtCrystalUse(true);
}

void loop() {
    sensors_event_t accelData;
    bno.getEvent(&accelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    // Send raw binary data (3 floats)
    SerialBT.write((uint8_t*)&accelData.acceleration.x, sizeof(float));
    SerialBT.write((uint8_t*)&accelData.acceleration.y, sizeof(float));
    SerialBT.write((uint8_t*)&accelData.acceleration.z, sizeof(float));
    // delay(500); // Wait 0.5 seconds before repeating
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
