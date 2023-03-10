#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
/*
   Ce code calcule la distance entre un point et notre position
   Et affiche sur un écran OLED
   Basé sur KitchenSink.ino de Mikal Hart
*/
static const int RXPin = D4, TXPin = D3;  //D4 bleu et D3 vert
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// For stats that happen every 5 seconds
unsigned long last = 0UL;

double distanceToTRESORtemp = 0;
static const double TRESOR_LAT = 50.45405, TRESOR_LON = 3.949944;  //Coordonnées du trésor(ici, le Beffroi de Mons)

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("DistanceParRapportA1Point.ino"));
  Serial.println(F("Calcule la distance entre un point et notre position"));
  Serial.print(F("Testing TinyGPSPlus library v. "));
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop() {
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());

  if (gps.location.isUpdated()) {
    Serial.print(F("LOCATION  Lat:"));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" Long="));
    Serial.println(gps.location.lng(), 6);
  }

  else if (gps.course.isUpdated()) {
    Serial.print(F("COURSE     "));
    Serial.print(F("Deg="));
    Serial.println(gps.course.deg());
  }

  else if (gps.altitude.isUpdated()) {
    Serial.print(F("ALTITUDE   "));
    Serial.print(F(" Meters="));
    Serial.println(gps.altitude.meters());
  }

  else if (millis() - last > 5000) {
    if (gps.charsProcessed() < 10)
      Serial.println(F("WARNING: No GPS data.  Check wiring."));
    else {
      if (gps.location.isValid()) {
        double distanceToTRESOR =
          TinyGPSPlus::distanceBetween(
            gps.location.lat(),
            gps.location.lng(),
            TRESOR_LAT,
            TRESOR_LON);

        Serial.print(F("TRESOR     Distance="));
        Serial.println(distanceToTRESOR / 1000, 6);

        if (distanceToTRESORtemp > distanceToTRESOR) {
          Serial.println(F("Vous chauffez"));
        } else if (distanceToTRESORtemp < distanceToTRESOR) {
          Serial.println(F("Vous vous éloignez"));
        } else {
          Serial.println(F("Bougez svp"));
        }

        distanceToTRESORtemp = distanceToTRESOR;

      }
      last = millis();
    }
  }
}
