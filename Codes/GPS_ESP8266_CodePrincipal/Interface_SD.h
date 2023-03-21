#ifndef Interface_SD_h
  #define Interface_SD_h
  #include <Arduino.h>
  // Librairies pour SD
  #include <SD.h>  //used for the microSD card functionality

  #include <TinyGPSPlus.h>
  void SD_Init(int chipSelect);
  void CreateFileWithHeader(String PathFileName, String Header);
  String GPSFormat(TinyGPSPlus gps);
  void WritePath(String Data, String PathFileName);
  //Lit le fichier avec les étapes pour récupérer la prochaine étape
  String ReadWaypoints(String WaypointsFileName);
#endif