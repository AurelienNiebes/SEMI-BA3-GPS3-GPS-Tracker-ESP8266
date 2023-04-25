#include <SoftwareSerial.h>
const int RXPin = D4, TXPin = D3;    //D2 bleu et D1 vert
const uint32_t GPSBaud = 9600; //Default baud of NEO-6M is 9600 ! Notre modèle a un baudrate de 9600
SoftwareSerial SerialGPS(RXPin, TXPin);
void setup() {
  Serial.begin(115200);
  SerialGPS.begin(GPSBaud);
  Serial.println("LE programme commence");
}

void loop() {
  while (SerialGPS.available() > 0)
    Serial.println(SerialGPS.readStringUntil(13));
}
