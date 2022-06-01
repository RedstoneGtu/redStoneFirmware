#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SENSOR_COUNT (4)

const int SENSOR_PINS[SENSOR_COUNT] = {34, 35, 32, 33};
const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char *SENSOR_CHARACTERISTIC_UUID = "89659080-bfcf-11ec-9d64-0242ac120002";

BLEServer *pServer;
BLEService *pService;
BLEAdvertising *pAdvertising;
BLECharacteristic *pCharacteristics;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("Group 2 RedStone");
  
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristics = pService->createCharacteristic(SENSOR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
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
  String temp = "";  
  for(int j = 0; j < SENSOR_COUNT; j++){
    temp += analogRead(SENSOR_PINS[j]);
    if(j != SENSOR_COUNT-1)
      temp += ",";
  }
  pCharacteristics->setValue(std::string(String(temp).c_str()));
  Serial.print(temp);
  Serial.println("");
  delay(20);
}
