#include "BluetoothSerial.h"


const char *pin = "1234";  // Cambiar para mejor seguridad
String device_name = "Julian's ESP32";


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Serial Bluetooth not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif


BluetoothSerial SerialBT;

void serialToBluetoothTask(void *pvParameters) {
  (void)pvParameters;
  while (1) {
    if (Serial.available()) {
      SerialBT.write(Serial.read());
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void bluetoothToSerialTask(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    if (SerialBT.available()) {
      Serial.write(SerialBT.read());
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);
  Serial.printf("The device with name \"%s\" is started. \nNow you can pair it with Bluetooh!\n", device_name.c_str());

#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif

  xTaskCreatePinnedToCore(
    serialToBluetoothTask,
    "serialToBluetoothTask",
    4096,
    NULL,
    1,
    NULL,
    1);

  xTaskCreatePinnedToCore(
    bluetoothToSerialTask,
    "bluetoothToSerialTask",
    4096,
    NULL,
    1,
    NULL,
    1);
}

void loop() {
  // put your main code here, to run repeatedly:
}
