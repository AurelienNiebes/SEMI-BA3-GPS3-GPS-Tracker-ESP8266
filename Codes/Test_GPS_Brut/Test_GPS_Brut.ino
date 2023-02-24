#include <SoftwareSerial.h>
const int RXPin = 4, TXPin = 5;   
const uint32_t GPSBaud = 9600; //Default baud of NEO-6M is 9600 ! Notre modèle a un baudrate de 9600
SoftwareSerial SerialGPS(RXPin, TXPin);
void setup() {
  Serial.begin(9600);
  SerialGPS.begin(GPSBaud);
}

void loop() {
  while (SerialGPS.available() > 0)
    Serial.print(SerialGPS.read());
  if (millis() > 5000 && gps.charsProcessed() < 10) //if there is no GPS data detected after 5s then the serial monitor will display “GPS NOT DETECTED!” message.
  {
    Serial.println("GPS NOT DETECTED!");
    while(true);
  }
}
