#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "FFT.h"

#define SENSOR_COUNT (4)
#define FFT_N (256)
#define TOTAL_TIME (126.0f)
#define FFT_TIME (10.5f)

const unsigned int DELAY_MS = (1000 * FFT_TIME) / (unsigned int)FFT_N;

struct FFT_Helper {
  fft_config_t *plan;
  float input[FFT_N];
  float output[FFT_N];
};

const int SENSOR_PINS[SENSOR_COUNT] = {34, 35, 32, 33};
const char *SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char *SENSOR_CHARACTERISTIC_UUID = "89659080-bfcf-11ec-9d64-0242ac120002";

BLEServer *pServer;
BLEService *pService;
BLEAdvertising *pAdvertising;
BLECharacteristic *pCharacteristics;

struct FFT_Helper fft_helper[SENSOR_COUNT];

float max_magnitude = 0;
float fundamental_freq = 0;
unsigned long last_time = 0;

void ble_setup() {
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

void write_ble() {
  String temp = "";  
  for(int j = 0; j < SENSOR_COUNT; ++j) {
    for (int i = 0; i < FFT_N; ++i) {
      temp += analogRead(SENSOR_PINS[j]);
      if(FFT_N - 1 != i) {
        temp += ",";
      }
    }
    if(SENSOR_COUNT - 1 != j) {
      temp += ";";
    }
  }
  pCharacteristics->setValue(std::string(String(temp).c_str()));
}

void fft_setup() {
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    fft_helper[i].plan = fft_init(FFT_N, FFT_REAL, FFT_FORWARD, fft_helper[i].input, fft_helper[i].output);
  }
}

void fft_destroy() {
  for (int i = 0; i < SENSOR_COUNT; ++i) {
    fft_destroy(fft_helper[i].plan);
  }
}

void setup() {
  Serial.begin(115200);
  ble_setup();
  last_time = micros() - DELAY_MS;
}

void loop() {
  fft_setup();
  for (int i = 0; i < FFT_N; ++i) {
    while (micros() - DELAY_MS < last_time) {
      delayMicroseconds(1);
    }
    last_time = micros();
    for(int j = 0; j < SENSOR_COUNT; ++j) {
      fft_helper[j].input[i] = analogRead(SENSOR_PINS[j]);
    }
  }
  for(int j = 0; j < SENSOR_COUNT; ++j) {
    fft_execute(fft_helper[j].plan);
  }
  write_ble();
  fft_destroy();
}
