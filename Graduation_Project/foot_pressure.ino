#include <ArduinoBLE.h>

BLEService PressureService("e25f6670-1b11-4929-a53e-44d0619a3882");
BLEFloatCharacteristic PressureLevel("cc04c592-bb36-40cc-bdae-cff44898c463", BLEWrite | BLERead);  

int sensor_pin = A3;
float sensor_value = 0;

void setup() {
  Serial.begin(9600);
  
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1); 
  }
  // set advertised local name and service UUID:
  BLE.setLocalName("leftpressure");
  BLE.setAdvertisedService(PressureService); 
  PressureService.addCharacteristic(PressureLevel); 
  BLE.addService(PressureService);
  PressureLevel.writeValue(sensor_value);

  // start advertising
  BLE.advertise();
  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()) {
      sensor_value = analogRead(sensor_pin);
      PressureLevel.writeValue(sensor_value);
      delay(100); 
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
