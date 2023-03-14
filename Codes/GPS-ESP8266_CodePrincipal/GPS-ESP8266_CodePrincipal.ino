#include "GPS-ESP8266_CodePrincipal.h"

static const int RXPin = D4, TXPin = D3;  //D4 bleu et D3 vert
const int chipSelect = D8 //SD
static const uint32_t SerialBaud = 115200;
static const uint32_t GPSBaud = 9600;

SoftwareSerial ss(RXPin, TXPin);
TinyGPSPlus gps;

void setup() {
 Serial.begin(SerialBaud);
}

void loop() {
 

}
