#include "GPS_ESP8266_CodePrincipal.h"

//FONCTIONS
//Initialisation de la communication série
void Serial_init(int BaudRate) {
  Serial.begin(BaudRate);
  while (!Serial) {
  }
  Serial.println("Communication serie initialisee");
}
const double Seuil = 50;
WayPoint Tresor = { 50.45405, 3.949944, "Beffroi", "Le plus haut point de Mons" };
// Communication avec le GPS
TinyGPSPlus GPS_Communication(TinyGPSPlus gps) {
  static int lastVerif = millis();
  static int lastCharProc = 0;
  gps = Obtain_GPS_Data(gps);
  if ((millis() - lastVerif) > 7000)  //if there is no GPS data detected after 5s then the serial monitor will display “GPS NOT DETECTED!” message.
  {
    if (gps.charsProcessed() < lastCharProc + 10) {
      Serial.println(F("GPS NOT DETECTED!"));
      //while(true);
      OLED_Clear();
      OLED_Print(2, 10, "Aucune donnee du GPS");
      OLED_Print(15, 25, "Est-il branche ?");
      OLED_Display();
    } else {
      Serial.println(F("Aucun Fix GPS"));
      OLED_Clear();
      OLED_Print(17, 5, "Aucun signal GPS");
      OLED_Print(17, 15, "Attendez un peu...");
      OLED_Print(25, 25, "Si vous etes a l'interieur, essayez de vous approcher d'une fenetre");
      OLED_Display();
    }
    lastCharProc = gps.charsProcessed();
    lastVerif = millis();
  }
  if (gps.time.isUpdated()) {  //Le temps est la première donnée à être actualisée
    gps = SerialPrintUpdatedData(gps);
    lastVerif = millis();
    OLED_Clear();
    OLED_Display();
  }
  return gps;
}
void PremiereEtape(String WaypointsFileName, int offset) {
  Tresor = ReadWaypoints(WaypointsFileName, offset);
}

// Sauvegarde données GPS
TinyGPSPlus SD_SauvegardeDonneesGPS(TinyGPSPlus gps, String PathFileName) {
  gps = Obtain_GPS_Data(gps);
  if (gps.location.isUpdated()) {  //Le temps est la première donnée à être actualisée
    gps = SerialPrintUpdatedData(gps);
    WritePath(GPSFormat(gps), PathFileName);
  }
  return gps;
}
const unsigned char Cup[] PROGMEM = {
  // 'cup-trophy-pixel, 50x55px
  32, 35,
  0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x3e, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0xfc,
  0xc3, 0x00, 0x00, 0xc3, 0xc3, 0x00, 0x00, 0xc3, 0xc3, 0x00, 0x00, 0xc3, 0xc3, 0x00, 0x00, 0xc3,
  0xc3, 0x00, 0x00, 0xc3, 0xc3, 0x00, 0x00, 0xc3, 0xc3, 0x00, 0x00, 0xc3, 0xc3, 0x00, 0x00, 0xc3,
  0xc3, 0x00, 0x00, 0xc3, 0x33, 0x00, 0x00, 0xcc, 0x33, 0x00, 0x00, 0xcc, 0x0f, 0x00, 0x00, 0xf0,
  0x0f, 0x00, 0x03, 0xf0, 0x00, 0xc0, 0x03, 0x80, 0x00, 0x20, 0x0e, 0x00, 0x00, 0x30, 0x0e, 0x00,
  0x00, 0x18, 0x18, 0x00, 0x00, 0x1c, 0x18, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x60, 0x00,
  0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x60, 0x00,
  0x00, 0x06, 0x60, 0x00, 0x00, 0x06, 0x78, 0x00, 0x00, 0x0c, 0x18, 0x00, 0x00, 0x38, 0x1e, 0x00,
  0x00, 0x30, 0x06, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x3f, 0xfe, 0x00
};

// Lecture des fichiers sur la carte SD
void SD_LectureFichiers(String WaypointsFileName, int offset) {
  static int last = millis();
  //gps=Obtain_GPS_Data(gps);
  if (millis() - last > 3000) {
    if (Tresor.latitude == -1) {
      OLED_Clear();
      OLED_Print(25, 2, "Felicitations ");
      drawRotatedBitmap(63, 45, Cup, 0);
      OLED_Print(10, 12, "Vous avez gagne !!");
      //OLED_Print(10, 34, "Le jeu est fini");
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
TinyGPSPlus OLED_DistanceChaudFroid_Jauge_et_Fleche(TinyGPSPlus gps, String WaypointsFileName, String PathFileName, int offset, boolean EtapesLues, boolean SDConnecte) {
  static int last = millis();
  static int lastVerif = millis();
  static int lastCharProc = 0;

  static double distanceToTRESOR;
  static double courseToTRESOR;
  gps = Obtain_GPS_Data(gps);
  if (Tresor.latitude < 0 && Tresor.nom.equals("Fin")) {  //TODO: potentiel problème si les latitudes de l'hémisphère SUD sont négatives
    Serial.println("Fini !");
    OLED_Clear();
    OLED_Print(25, 2, "Felicitations ");
    drawRotatedBitmap(63, 45, Cup, 0);
    OLED_Print(10, 12, "Vous avez gagne !!");
    //OLED_Print(10, 34, "Le jeu est fini");
    OLED_Display();
  } else {
    if ((millis() - lastVerif) > 7000) {  //if there is no GPS data detected after 7s then the serial monitor will display “GPS NOT DETECTED!” message.
      if (gps.charsProcessed() < lastCharProc+10) {    //TODO: Potentiel problème si le capteur a un dysfonctionnement après avoir bien fonctionné
        Serial.println(F("GPS NOT DETECTED!"));
        //while(true);
        OLED_Clear();
        OLED_Print(2, 10, "Aucune donnee du GPS");
        OLED_Print(15, 25, "Est-il branche ?");
        OLED_Display();
      } else {
        Serial.println(F("Aucun Fix GPS"));
        OLED_Clear();
        OLED_Print(17, 5, "Aucun signal GPS");
        OLED_Print(17, 15, "Attendez un peu...");
        OLED_Print(25, 25, "Si vous etes a l'interieur, essayez de vous approcher d'une fenetre");
        OLED_Display();
      }
      lastVerif = millis();
      lastCharProc = gps.charsProcessed();
    }
    //if (millis() - last > 500) {
    if (gps.location.isUpdated()) {  //Le temps est la première donnée à être actualisée
      gps = SerialPrintUpdatedData(gps);
      if (SDConnecte) {
        WritePath(GPSFormat(gps), PathFileName);
      }
      lastVerif = millis();
      if (gps.location.isValid()) {
        distanceToTRESOR = TinyGPSPlus::distanceBetween(
          gps.location.lat(), gps.location.lng(),
          Tresor.latitude, Tresor.longitude);

        courseToTRESOR = TinyGPSPlus::courseTo(
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
            Tresor.nom = "Fin";
          }
          last = millis();
        }

        if (millis() - last > 5000) {
          OLED_Clear();

          OLED_Print(35, 0, Tresor.nom);
          OLED_PrintDistance(0, 54, distanceToTRESOR);
          OLED_DrawJauge(5, 25, distanceToTRESOR);
          drawRotatedBitmap(100, 35, epd_bitmap_Fleche, (uint16_t)courseToTRESOR);
          OLED_TextSize(2);
          OLED_Print(95, 4, "N");

          OLED_Display();
          OLED_TextSize(1);
          //Serial.println();
          /*Serial.println(Tresor.latitude);
          Serial.println(Tresor.longitude);
          Serial.println(Tresor.nom);
          Serial.println(Tresor.description);*/
        }
      }
    }
  }
  //last = millis();
  //}
  return gps;
}