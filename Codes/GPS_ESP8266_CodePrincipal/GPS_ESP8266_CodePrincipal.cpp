#include "GPS_ESP8266_CodePrincipal.h"

//FONCTIONS
//Initialisation de la communication série
void Serial_init(int BaudRate) {
  Serial.begin(BaudRate);
  while (!Serial) {
  }
  Serial.println("Communication serie initialisee");
}
// Communication avec le GPS
TinyGPSPlus GPS_Communication(TinyGPSPlus gps) {
  gps=Obtain_GPS_Data(gps);
  if (gps.time.isUpdated()) {//Le temps est la première donnée à être actualisée
    SerialPrintUpdatedData(gps);
  }
  return gps;
}

// Google Maps
void Wifi_Google_Maps() {
}

// Sauvegarde données GPS
TinyGPSPlus SD_SauvegardeDonneesGPS(TinyGPSPlus gps, String PathFileName) {
  gps=Obtain_GPS_Data(gps);
  if (gps.location.isUpdated()) {//Le temps est la première donnée à être actualisée
    SerialPrintUpdatedData(gps);
    WritePath(GPSFormat(gps), PathFileName);
  }
  return gps;
}

// Lecture des fichiers sur la carte SD
void SD_LectureFichiers() {
}

// Suppression des fichiers sur la carte SD
void SD_SuppressionFichiers() {
}

const double TRESOR_LAT = 50.45405, TRESOR_LON = 3.949944;  //Coordonnées du trésor(ici, le Beffroi de Mons)
// Chasse au trésor distance
TinyGPSPlus OLED_DistanceChaudFroid(TinyGPSPlus gps, Adafruit_SSD1306 display) {
  static int last=millis();
  static double distanceToTRESORtemp = 0;
  gps=Obtain_GPS_Data(gps);
  if (gps.time.isUpdated()) {//Le temps est la première donnée à être actualisée
    SerialPrintUpdatedData(gps);
  }
  if (millis() - last > 5000) {
    if (gps.location.isValid()) {
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
      Serial.print(distanceToTRESOR / 1000, 6);
      Serial.print(F(" km Course-to="));
      Serial.print(courseToTRESOR, 6);
      Serial.print(F(" degrees ["));
      Serial.print(TinyGPSPlus::cardinal(courseToTRESOR));  //returns course in degrees (North=0, West=270) from position 1 to position 2
      Serial.println(F("]"));

      display.setCursor(0, 20);

      if (distanceToTRESORtemp > distanceToTRESOR) {
        Serial.println(F("Vous chauffez"));
        display.println("Vous chauffez");
      } else if (distanceToTRESORtemp < distanceToTRESOR) {
        Serial.println(F("Vous vous éloignez"));
        display.println("Vous vous eloignez");
      } else {
        Serial.println(F("Bougez svp"));
        display.println("Bougez svp");
      }
      display.display();
      display.clearDisplay();

      distanceToTRESORtemp = distanceToTRESOR;
      last = millis();
      Serial.println();
    }
  }
  return gps;
}
//Fleche vers le haut de 40 par 56 pixels
const PROGMEM static uint8_t epd_bitmap_Fleche[] = {
  40, 56,
  0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
  0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x03,
  0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff,
  0xf0, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xfc,
  0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
  0x01, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xc0, 0x07,
  0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff,
  0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff,
  0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00,
  0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00,
  0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f,
  0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff,
  0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe,
  0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00,
  0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00,
  0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f,
  0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00
};
//Chasse au trésor orientation
TinyGPSPlus OLED_OrientationFleches(TinyGPSPlus gps, Adafruit_SSD1306 display) {
  static int last=millis();
  static double distanceToTRESORtemp = 0;
  gps=Obtain_GPS_Data(gps);
  if (gps.time.isUpdated()) {//Le temps est la première donnée à être actualisée
    SerialPrintUpdatedData(gps);
  }
  if (millis() - last > 5000) {
    if (gps.location.isValid()) {
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
      Serial.print(distanceToTRESOR / 1000, 6);
      Serial.print(F(" km Course-to="));
      Serial.print(courseToTRESOR, 6);
      Serial.print(F(" degrees ["));
      Serial.print(TinyGPSPlus::cardinal(courseToTRESOR));  //NW, etc.
      Serial.println(F("]"));

      //Flèche tournante
      display.clearDisplay();  // Clear the display buffer
      drawRotatedBitmap(display, 80, 32, epd_bitmap_Fleche, (uint16_t)courseToTRESOR);
      display.display();  // Now update the display with the buffer
      
      last = millis();
      Serial.println();
    }
  }
  return gps;
}