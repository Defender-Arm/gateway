#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

// BLE UUIDs for service and characteristics
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID_X "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHAR_UUID_Y "0e8f1722-1b3e-4d9b-a8ca-cdf0350b7c1d"
#define CHAR_UUID_Z "6e400001-b5a3-f393-e0a9-e50e24dcca9e"

BLECharacteristic *charX;
BLECharacteristic *charY;
BLECharacteristic *charZ;

void setup() {
    Serial.begin(115200);

    if (!bno.begin()) {
        Serial.println("Could not find a valid BNO055 sensor, check wiring!");
        while (1);
    }
    delay(1000);
    bno.setExtCrystalUse(true);

    BLEDevice::init("ESP32_BLE");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create BLE characteristics for each axis
    charX = pService->createCharacteristic(CHAR_UUID_X, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    charY = pService->createCharacteristic(CHAR_UUID_Y, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    charZ = pService->createCharacteristic(CHAR_UUID_Z, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("BLE setup complete. Advertising...");
}

void loop() {
    sensors_event_t linearAccelData;
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    // Update characteristics with sensor data
    charX->setValue(String(linearAccelData.acceleration.x).c_str());
    charY->setValue(String(linearAccelData.acceleration.y).c_str());
    charZ->setValue(String(linearAccelData.acceleration.z).c_str());

    charX->notify();
    charY->notify();
    charZ->notify();

    delay(500);
}
