#include <Arduino.h>
#include <CurieBLE.h>

const int ledPin = 3;
const int pirPin = 4;

int pirState = LOW;
int val = 0;                  // variable for reading pir sensor

const uint8_t NO_ALERT = 0;
const uint8_t HIGH_ALERT = 2;

BLEPeripheral blePeripheral;
BLEService alertService("1802");

BLEByteCharacteristic alertLevel("2A06", BLERead | BLENotify);

void setupBluetooth() {
  alertLevel.setValueLE(NO_ALERT);
  blePeripheral.setLocalName("PIR Sensor");
  blePeripheral.setAdvertisedServiceUuid(alertService.uuid());
  blePeripheral.addAttribute(alertService);
  blePeripheral.addAttribute(alertLevel);
  
  blePeripheral.begin();
}

void setupPIR () {
  pinMode(pirPin, INPUT);
}

void readPIR() {
  val = digitalRead(pirPin);     // read input value
  if (val == HIGH) {             // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {      
      // We only want to print on the output change, not state
      alertLevel.setValueLE(HIGH_ALERT);
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      alertLevel.setValueLE(NO_ALERT);
      pirState = LOW;
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

  setupPIR();
  setupBluetooth();

  Serial.println("Bluetooth device active.  Waiting for connection.");
}

void loop() {
  readPIR();
}