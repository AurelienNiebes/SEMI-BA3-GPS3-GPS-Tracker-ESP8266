#ifndef GPS_ESP8266_CodePrincipal_H
#define GPS_ESP8266_CodePrincipal_H
//LIBRAIRIES
#include <Arduino.h>
#include <list>
//Librairies pour le Wifi
#include <ESP8266WiFi.h>

#include "Capteur_GPS.h"
#include "Interface_SD.h"
#include "OLED.h"

void Serial_init(int BaudRate);
// Communication avec le GPS
TinyGPSPlus GPS_Communication(TinyGPSPlus gps);
void PremiereEtape(String WaypointsFileName,int offset);

// Sauvegarde données GPS
TinyGPSPlus SD_SauvegardeDonneesGPS(TinyGPSPlus gps, String PathFileName);

// Lecture des fichiers sur la carte SD
void SD_LectureFichiers(String WaypointsFileName, int offset);

// Chasse au trésor distance
TinyGPSPlus OLED_DistanceChaudFroid_Jauge_et_Fleche(TinyGPSPlus gps,String WaypointsFileName, String PathFileName, int offset, boolean EtapesLues, boolean SDConnecte);

#endif
