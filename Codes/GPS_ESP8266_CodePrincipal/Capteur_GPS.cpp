#include "Capteur_GPS.h"

const int RXPin = D4, TXPin = D3;  //D4 bleu et D3 vert

SoftwareSerial SerialGPS(RXPin, TXPin);
void GPS_Init(int GPSBaud) {
  SerialGPS.begin(GPSBaud);
  Serial.println(F("GPS connecté"));
}
TinyGPSPlus Obtain_GPS_Data(TinyGPSPlus gps) {
  static int last = millis();
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }
  if ((millis() - last) > 5000 && gps.charsProcessed() < 10)  //if there is no GPS data detected after 5s then the serial monitor will display “GPS NOT DETECTED!” message.
  {
    Serial.println(F("GPS NOT DETECTED!"));
    //while(true);
    last = millis();
  }
  return gps;
}
void SerialPrintUpdatedData(TinyGPSPlus gps) {
  if (gps.location.isUpdated()) {
    if (gps.location.isValid()) {
      Serial.print(F("- latitude: "));
      Serial.println(gps.location.lat());

      Serial.print(F("- longitude: "));
      Serial.println(gps.location.lng());

      Serial.print(F("- altitude: "));
      if (gps.altitude.isValid())
        Serial.println(gps.altitude.meters());
      else
        Serial.println(F("INVALID"));
    } else {
      Serial.println(F("- location: INVALID"));
    }
  }
  if (gps.speed.isUpdated()) {
    if (gps.speed.isValid()) {
      Serial.print(F("- speed: "));
      Serial.print(gps.speed.kmph());
      Serial.println(F(" km/h"));
    } else {
      Serial.println(F("INVALID"));
    }
  }

  if (gps.date.isUpdated() && gps.time.isUpdated()) {
    Serial.print(F("- GPS date&time: "));

    Serial.print(gps.date.year());
    Serial.print(F("-"));
    Serial.print(gps.date.month());
    Serial.print(F("-"));
    Serial.print(gps.date.day());
    Serial.print(F(" "));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    Serial.println(gps.time.second());
  } else {
    Serial.println(F("INVALID"));
  }
  Serial.println();
}