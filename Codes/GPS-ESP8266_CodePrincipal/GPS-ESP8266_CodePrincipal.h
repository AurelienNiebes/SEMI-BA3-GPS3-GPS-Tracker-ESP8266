#ifndef GPS-ESP8266_CodePrincipal_H
  #define GPS-ESP8266_CodePrincipal_H
  
  #include <Arduino.h> 
  
// Communication avec le GPS
void GPS_Communication();

// Google Maps
void Wifi_Google_Maps();

// Sauvegarde données GPS
void SD_SauvegardeDonneesGPS();

// Lecture des fichiers sur la carte SD
void SD_LectureFichiers();

// Suppression des fichiers sur la carte SD
void SD_SuppressionFichiers();

// Chasse au trésor distance
void OLED_DistanceChaudFroid();

//Chasse au trésor orientation
void OLED_OrientationFleches(); 

#endif
