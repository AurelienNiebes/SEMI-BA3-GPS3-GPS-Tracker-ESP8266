#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
/*
   This sample code demonstrates just about every built-in operation of TinyGPSPlus (TinyGPSPlus).
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = D2, TXPin = D1; //D2 bleu et D1 vert
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// For stats that happen every 5 seconds
unsigned long last = 0UL;

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("KitchenSink.ino"));
  Serial.println(F("Demonstrating nearly every feature of TinyGPSPlus"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop()
{
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());

  if (gps.location.isUpdated())
  {
    Serial.print(F("LOCATION  Lat:"));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" Long="));
    Serial.println(gps.location.lng(), 6);
  }

  else if (gps.course.isUpdated())
  {
    Serial.print(F("COURSE     "));
    Serial.print(F("Deg="));
    Serial.println(gps.course.deg());
  }

  else if (gps.altitude.isUpdated())
  {
    Serial.print(F("ALTITUDE   "));
    Serial.print(F(" Meters="));
    Serial.println(gps.altitude.meters());
  }

  else if (millis() - last > 5000)
  {
    Serial.println();
    if (gps.location.isValid())
    {
      static const double TRESOR_LAT = 50.4623627, TRESOR_LON = 4.0498483;
      double distanceToTRESOR =
        TinyGPSPlus::distanceBetween(
          gps.location.lat(),
          gps.location.lng(),
          TRESOR_LAT, 
          TRESOR_LON);
      double courseToTRESOR =
        TinyGPSPlus::courseTo(
          gps.location.lat(),
          gps.location.lng(),
          TRESOR_LAT, 
          TRESOR_LON);

      Serial.print(F("TRESOR     Distance="));
      Serial.print(distanceToTRESOR/1000, 6);
      Serial.print(F(" km Course-to="));
      Serial.print(courseToTRESOR, 6);
      Serial.print(F(" degrees ["));
      Serial.print(TinyGPSPlus::cardinal(courseToTRESOR));
      Serial.println(F("]"));

      Serial.print(F("Vous chauffez"));
      Serial.print(F("Vous vous éloignez"));
    }

    if (gps.charsProcessed() < 10)
      Serial.println(F("WARNING: No GPS data.  Check wiring."));

    last = millis();
    Serial.println();
  }
}
