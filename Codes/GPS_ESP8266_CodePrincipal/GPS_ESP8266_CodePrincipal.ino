#include "GPS_ESP8266_CodePrincipal.h"
const int chipSelect = D8;         //SD

const uint32_t SerialBaud = 115200;
const uint32_t GPSBaud = 9600;

const String WaypointsFileName = "GPS_Waypoints.txt";
const String PathFileName = "GPS_Path.txt";

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
TinyGPSPlus gps;
std::list<WayPoint> Etapes;

int choix = 3;  //switch
void setup() {
  Serial_init(SerialBaud);
  GPS_Init(GPSBaud);
  SD_Init(chipSelect);
  OLED_Init(display);

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
  if(Serial.available()){
    choix = Serial.read()-48;
  }
  switch (choix) {
    case 1:
      GPS_Communication(gps);
      break;

    case 2:
      Wifi_Google_Maps();
      break;

    case 3:
      SD_SauvegardeDonneesGPS(gps, PathFileName);
      break;

    case 4:
      SD_LectureFichiers();
      break;

    case 5:
      SD_SuppressionFichiers();
      break;

    case 6:
      OLED_DistanceChaudFroid(gps, display);
      break;

    case 7:
      OLED_OrientationFleches(gps, display);
      break;

    default: Serial.println(F("Erreur: Choix inexistant"));
  }
}
