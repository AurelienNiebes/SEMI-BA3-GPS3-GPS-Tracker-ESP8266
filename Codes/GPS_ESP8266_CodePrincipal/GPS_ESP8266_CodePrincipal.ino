#include "GPS_ESP8266_CodePrincipal.h"
const int chipSelect = D8;  //SD

const uint32_t SerialBaud = 115200;
const uint32_t GPSBaud = 9600;

const String WaypointsFileName = "GPS_Waypoints.txt";
const String PathFileName = "GPS_Path.txt";


TinyGPSPlus gps;
std::list<WayPoint> Etapes;

bool SDConnecte = false;
bool EtapesLues=false;
int choix = 6;  //switch
const String HeaderAtt="type\tlatitude\tlongitude\tname\tdesc";
int offset=0;

void setup() {
  Serial_init(SerialBaud);
  GPS_Init(GPSBaud);
  OLED_Init();

  OLED_Display();

  delay(2000);
  OLED_Clear();
  SDConnecte = SD_Init(chipSelect);
  if (SDConnecte) {
    if (SD.exists(WaypointsFileName)) {
      File WaypointsFile = SD.open(WaypointsFileName, FILE_READ);
      if (WaypointsFile) {
        String header = WaypointsFile.readStringUntil('\n');
        header=header.substring(3);
        Serial.println("Header:");
        Serial.println(header);
        Serial.println(header);

        for(int i=0; i<header.length();i++){
          Serial.print(header[i]);
          Serial.print(header[i]==HeaderAtt[i]);
          Serial.println();
        }

        if (header.equals(HeaderAtt)) {
          Serial.println(F("Fichier d'étapes trouvé et valide"));
          EtapesLues=true;
          offset=WaypointsFile.position();
          PremiereEtape(WaypointsFileName, offset);
        } else {
          Serial.println(F("Fichier d'étapes non valide"));
          OLED_Clear();
          OLED_Print(20, 32, "Fichier d'etapes non valide.\nChargement de l\'etape par defaut...");
          OLED_Display();
          delay(4000);
        }
      } else {
        Serial.println(F("Fichier d'étapes non trouvé"));
        OLED_Clear();
        OLED_Print(20, 32, "Fichier d'etapes inexistant.\nChargement de l\'etape par defaut...");
        OLED_Display();
        delay(4000);
      }
    }
    if (!SD.exists(PathFileName)) {
      CreateFileWithHeader(PathFileName, F("Latitude, Longitude, Altitude, Time"));
    }
  }
  else{
    Serial.println(F("Lecteur SD defaillant"));
    OLED_Clear();
    OLED_Print(2, 8, "Lecteur SD defaillant");
    OLED_Print(2, 20, "Chargement de l\'etape par defaut...");
    OLED_Print(4, 38, "(Le chemin suivi ne sera pas sauvegarde)");
    OLED_Display();
    delay(10000);
  }
}

void loop() {
  static int last = millis();
  if (Serial.available()) {
    choix = Serial.read() - 48;
    Serial.println(choix);
  }
  switch (choix) {
    case 1:
      OLED_Clear();
      OLED_Display();
      gps = GPS_Communication(gps);
      break;

    case 2:
      OLED_Clear();
      OLED_Display();
      Wifi_Google_Maps();
      break;

    case 3:
      if(SDConnecte){
        gps = SD_SauvegardeDonneesGPS(gps, PathFileName);
      }
      else{
        Serial.println(F("Le programme ne peut etre execute"));
        OLED_Clear();
        OLED_Print(6, 24, "Le programme ne peut etre execute");
        OLED_Display();
      }
      break;

    case 4:
      if(SDConnecte&&EtapesLues){
        SD_LectureFichiers(WaypointsFileName, offset);
      }
      else{
        Serial.println(F("Le programme ne peut etre execute"));
        OLED_Clear();
        OLED_Print(6, 24, "Le programme ne peut etre execute");
        OLED_Display();
      }
      break;

    case 6:
      gps = OLED_DistanceChaudFroid_Jauge_et_Fleche(gps, WaypointsFileName, PathFileName, offset, EtapesLues, SDConnecte);
      break;

    case 7:
      gps = OLED_OrientationFleches(gps);
      break;

    default:
      if (millis() - last > 5000) {
        Serial.println(F("Erreur: Choix inexistant"));
        last = millis();
      }
  }
}
