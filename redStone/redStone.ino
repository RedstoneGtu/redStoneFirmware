#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define SENSOR_COUNT (4)
#define DELAY_MS (100)
#define TOTAL_TIME (126000)

const int SENSOR_PINS[SENSOR_COUNT] = {34, 35, 32, 33};

unsigned long timer = 0;
BluetoothSerial SerialBT;

void ble_setup() {
  SerialBT.begin("Group 2 RedStone");
}

void setup() {
  Serial.begin(115200);
  ble_setup();
  timer = millis();
}

void loop() {
  for(int j = 0; j < SENSOR_COUNT; ++j) {
    int val = analogRead(SENSOR_PINS[j]);
    Serial.print(val);
    SerialBT.print(val);
    if (SENSOR_COUNT - 1 != j) {
      Serial.print(",");
      SerialBT.print(",");
    }
  }
  Serial.print("\n");
  SerialBT.print(";\n");
  delay(DELAY_MS);
}
