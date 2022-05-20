#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SENSOR_COUNT (4)

const int SENSOR_PINS[SENSOR_COUNT] = {34, 35, 32, 33};
const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char *SENSOR_CHARACTERISTIC_UUIDS[SENSOR_COUNT] = {
  "89659080-bfcf-11ec-9d64-0242ac120002",
  "8965938c-bfcf-11ec-9d64-0242ac120002",
  "89659508-bfcf-11ec-9d64-0242ac120002",
  "89659652-bfcf-11ec-9d64-0242ac120002"
};

BLEServer *pServer;
BLEService *pService;
BLEAdvertising *pAdvertising;
BLECharacteristic *pCharacteristics[SENSOR_COUNT];

void setup() {
  Serial.begin(115200);
  BLEDevice::init("Group 2 RedStone");
  
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    pCharacteristics[i] = pService->createCharacteristic(
      SENSOR_CHARACTERISTIC_UUIDS[i],
      BLECharacteristic::PROPERTY_READ
    );
  }
  pService->start();
  
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void loop() {
  uint32_t vals[SENSOR_COUNT] = {0};
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    vals[i] = analogRead(SENSOR_PINS[i]);
    pCharacteristics[i]->setValue(vals[i]);
    Serial.print(vals[i]);
    Serial.print(",");
  }
  Serial.println(" ");
  delay(50);
}
