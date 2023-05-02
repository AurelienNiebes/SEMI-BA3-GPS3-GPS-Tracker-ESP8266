#include "GPS_ESP8266_CodePrincipal.h"

//FONCTIONS
//Initialisation de la communication série
void Serial_init(int BaudRate) {
  Serial.begin(BaudRate);
  while (!Serial) {
  }
  Serial.println("Communication serie initialisee");
}
const double Seuil = 100;
const double TRESOR_LAT = 50.45405, TRESOR_LON = 3.949944;  //Coordonnées du trésor(ici, le Beffroi de Mons)
WayPoint Tresor = { 50.45405, 3.949944, "Beffroi", "Le plus haut point de Mons" };
// Communication avec le GPS
TinyGPSPlus GPS_Communication(TinyGPSPlus gps) {
  gps = Obtain_GPS_Data(gps);
  if (gps.time.isUpdated()) {  //Le temps est la première donnée à être actualisée
    SerialPrintUpdatedData(gps);
  }
  return gps;
}
void PremiereEtape(String WaypointsFileName,int offset){
  Tresor = ReadWaypoints(WaypointsFileName, offset);
}
// Google Maps
void Wifi_Google_Maps() {
}

// Sauvegarde données GPS
TinyGPSPlus SD_SauvegardeDonneesGPS(TinyGPSPlus gps, String PathFileName) {
  gps = Obtain_GPS_Data(gps);
  if (gps.location.isUpdated()) {  //Le temps est la première donnée à être actualisée
    SerialPrintUpdatedData(gps);
    WritePath(GPSFormat(gps), PathFileName);
  }
  return gps;
}

// Lecture des fichiers sur la carte SD
void SD_LectureFichiers(String WaypointsFileName, int offset) {
  static int last = millis();
  //gps=Obtain_GPS_Data(gps);
  if (millis() - last > 3000) {
    if (Tresor.latitude == -1) {
      OLED_Clear();
      OLED_Print(20, 32, "Derniere etape atteinte !");
      OLED_Print(20, 40, "Felecitations !");
      OLED_Display();
    } else {
      Serial.println("Etape atteinte");
      OLED_Clear();
      OLED_Print(20, 32, "Bravo !");
      OLED_Print(2, 40, Tresor.description);
      OLED_Display();

      Tresor = ReadWaypoints(WaypointsFileName, offset);
      Serial.println(Tresor.nom);
      Serial.println(Tresor.latitude);

      last = millis();
    }
  }
}

//Fleche vers le haut de 40 par 56 pixels
/*const PROGMEM static uint8_t epd_bitmap_Fleche[] = {
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
};*/
const PROGMEM static uint8_t epd_bitmap_Fleche[] = {
  // 'Fleche, 20x32px
  16, 32,
  0x01, 0x80, 0x01, 0x80, 0x03, 0xc0, 0x03, 0xc0, 0x07, 0xe0, 0x07, 0xe0, 0x0f, 0xf0, 0x0f, 0xf0,
  0x1f, 0xf8, 0x1f, 0xf8, 0x3f, 0xfc, 0x3f, 0xfc, 0x7f, 0xfe, 0x7f, 0xfe, 0xff, 0xff, 0x0f, 0xf0,
  0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0,
  0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x00, 0x00
};
// Chasse au trésor distance
TinyGPSPlus OLED_DistanceChaudFroid_Jauge_et_Fleche(TinyGPSPlus gps,String WaypointsFileName, String PathFileName, int offset, boolean EtapesLues, boolean SDConnecte) {
  static int last = millis();
  gps = Obtain_GPS_Data(gps);
  /*if ((millis() - last) > 5000 && gps.charsProcessed() < 10)  //if there is no GPS data detected after 5s then the serial monitor will display “GPS NOT DETECTED!” message.
  {
    Serial.println(F("GPS NOT DETECTED!"));
    //while(true);
    OLED_Print(10, 10, "Aucune donnees du GPS");
    OLED_Print(10, 20, "Est-il branche ?");
    last = millis();
  }*/
  if (millis() - last > 500) {
    if (Tresor.latitude == -1) {
      Serial.println("Fini !");
      OLED_Clear();
      OLED_Print(10, 10, "Felicitations ");
      OLED_Print(10, 22, "Vous avez gagne !!\n");
      OLED_Print(10, 34, "Le jeu est fini");
      OLED_Display();
    } else {
      if (gps.location.isUpdated()) {  //Le temps est la première donnée à être actualisée
        SerialPrintUpdatedData(gps);
        if(SDConnecte){
          WritePath(GPSFormat(gps), PathFileName);
        }
      }
      if (gps.location.isUpdated() && gps.location.isValid()) {
        double distanceToTRESOR = TinyGPSPlus::distanceBetween(
          gps.location.lat(), gps.location.lng(),
          Tresor.latitude, Tresor.longitude);

        double courseToTRESOR = TinyGPSPlus::courseTo(
          gps.location.lat(), gps.location.lng(),
          Tresor.latitude, Tresor.longitude);

        Serial.print(F("TRESOR     Distance="));
        Serial.print(distanceToTRESOR / 1000, 6);
        Serial.print(F(" km Course-to="));
        Serial.print(courseToTRESOR, 6);
        Serial.print(F(" degrees ["));
        Serial.print(TinyGPSPlus::cardinal(courseToTRESOR));  //returns course in degrees (North=0, West=270) from position 1 to position 2
        Serial.println(F("]"));
        if (distanceToTRESOR < Seuil) {  //Passage à la prochaine étape
          OLED_Clear();
          OLED_Print(35, 0, "Bravo !");
          OLED_Print(2, 20, Tresor.description);
          OLED_Display();
          if (EtapesLues) {
            Tresor = ReadWaypoints(WaypointsFileName, offset);
          } else {
            Tresor.latitude = -1;
            Tresor.longitude = -1;
          }
        }
        OLED_Clear();
        OLED_Print(35, 0, Tresor.nom);
        OLED_PrintDistance(0, 54, distanceToTRESOR);
        OLED_DrawJauge(5, 25, distanceToTRESOR);
        drawRotatedBitmap(100, 28, epd_bitmap_Fleche, (uint16_t)courseToTRESOR);
        OLED_Display();
      }
    }
      last = millis();
      Serial.println();
  }
  return gps;
}
//Chasse au trésor orientation
TinyGPSPlus OLED_OrientationFleches(TinyGPSPlus gps) {
  static int last = millis();
  gps = Obtain_GPS_Data(gps);
  if (millis() - last > 5000) {
    if (gps.time.isUpdated()) {  //Le temps est la première donnée à être actualisée
      SerialPrintUpdatedData(gps);
    }
    if (gps.location.isUpdated()) {
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
      OLED_Clear();  // Clear the display buffer
      drawRotatedBitmap(80, 32, epd_bitmap_Fleche, (uint16_t)courseToTRESOR);
      OLED_Display();  // Now update the display with the buffer

      last = millis();
      Serial.println();
    }
  }
  return gps;
}