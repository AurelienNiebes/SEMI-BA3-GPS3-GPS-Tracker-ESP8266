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
const unsigned char Cup [] PROGMEM = {
	// 'cup-trophy-bit-pixel-graphics-icon-pixel-art-style-game-assets-bit-sprite-isolated-vector-illustration-eps-cup-trophy-bit-pixel-231952392, 50x55px
	32,35,
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
void setup() {
  Serial_init(SerialBaud);
  GPS_Init(GPSBaud);
  OLED_Init();

  OLED_Display();
  delay(2000);
  drawRotatedBitmap(63, 45 , Cup, 0);
  OLED_Display();
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
