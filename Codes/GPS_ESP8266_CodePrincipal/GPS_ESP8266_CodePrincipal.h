#ifndef GPS_ESP8266_CodePrincipal_H
#define GPS_ESP8266_CodePrincipal_H
//LIBRAIRIES
#include <Arduino.h>
//Librairies pour le Wifi
#include <ESP8266WiFi.h>

#include "Capteur_GPS.h"
#include "Interface_SD.h"
#include "OLED.h"

void Serial_init(int BaudRate);
// Communication avec le GPS
void GPS_Communication(TinyGPSPlus gps);

// Google Maps
void Wifi_Google_Maps();

// Sauvegarde données GPS
void SD_SauvegardeDonneesGPS(TinyGPSPlus gps, String PathFileName);

// Lecture des fichiers sur la carte SD
void SD_LectureFichiers();

// Suppression des fichiers sur la carte SD
void SD_SuppressionFichiers();

// Chasse au trésor distance
void OLED_DistanceChaudFroid(TinyGPSPlus gps, Adafruit_SSD1306 display);

//Chasse au trésor orientation
void OLED_OrientationFleches(TinyGPSPlus gps, Adafruit_SSD1306 display);

#endif
