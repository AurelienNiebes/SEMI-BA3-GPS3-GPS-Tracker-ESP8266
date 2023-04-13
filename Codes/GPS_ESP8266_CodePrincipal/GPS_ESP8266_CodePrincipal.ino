#include "GPS_ESP8266_CodePrincipal.h"
const int chipSelect = D8;         //SD

const uint32_t SerialBaud = 115200;
const uint32_t GPSBaud = 9600;

const String WaypointsFileName = "GPS_Waypoints.txt";
const String PathFileName = "GPS_Path.txt";


TinyGPSPlus gps;
std::list<WayPoint> Etapes;

int choix = 1;  //switch
void setup() {
  Serial_init(SerialBaud);
  GPS_Init(GPSBaud);
  SD_Init(chipSelect);
  OLED_Init();

  if(SD.exists(PathFileName)){
    CreateFileWithHeader(PathFileName, F("Latitude, Longitude, Altitude, Time"));
  }

  if(SD.exists(WaypointsFileName)){
    File WaypointsFile = SD.open(WaypointsFileName, FILE_READ);
    if(WaypointsFile){
      String header = WaypointsFile.readStringUntil('\n');
      Serial.println("Header:");
      Serial.println(header);
      Serial.println();
      
      if(header.equals("type	latitude	longitude	name	desc")){
        Serial.println(F("Fichier d'étapes trouvé et valide"));
      }
      else{
        Serial.println(F("Fichier d'étapes non valide"));
      }
    }
    else{
      Serial.println(F("Fichier d'étapes non trouvé"));
    }
  }
}

void loop() {
  static int last=millis();
  if(Serial.available()){
    choix = Serial.read()-48;
    Serial.println(choix);
  }
  switch (choix) {
    case 1:
      gps=GPS_Communication(gps);
      break;

    case 2:
      Wifi_Google_Maps();
      break;

    case 3:
      gps=SD_SauvegardeDonneesGPS(gps, PathFileName);
      break;

    case 4:
      SD_LectureFichiers();
      break;

    case 5:
      SD_SuppressionFichiers();
      break;

    case 6:
      gps=OLED_DistanceChaudFroid(gps);
      break;

    case 7:
      gps=OLED_OrientationFleches(gps);
      break;

    default:
      if (millis() - last > 5000) {
        Serial.println(F("Erreur: Choix inexistant"));
        last=millis();
      }
  }
}
