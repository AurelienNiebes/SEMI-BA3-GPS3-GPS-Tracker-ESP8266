#include "GPS-ESP8266_CodePrincipal.h"

static const int RXPin = D4, TXPin = D3;  //D4 bleu et D3 vert
const int chipSelect = D8 //SD

static const uint32_t SerialBaud = 115200;
static const uint32_t GPSBaud = 9600;

int choix; //switch

SoftwareSerial ss(RXPin, TXPin);
TinyGPSPlus gps;

void setup() {
 Serial.begin(SerialBaud);
}

void loop() {
  
switch (choix) {

  case 1:
  GPS_Communication();
   break; 
        
  case 2:
  Wifi_Google_Maps();
   break; 
         
  case 3:
  SD_SauvegardeDonneesGPS();
   break; 
        
  case 4:
  SD_LectureFichiers();
   break;  
        
  case 5:
  SD_SuppressionFichiers();
   break;  
        
  case 6:
  OLED_DistanceChaudFroid();
        break;
        
  case 7:
  OLED_OrientationFleches(); 
        break;   
                     
  default: Serial.println(F("Erreur: Choix inexistant"));
}

}
