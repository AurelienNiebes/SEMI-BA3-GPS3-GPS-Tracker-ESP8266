#ifndef Capteur_GPS_h
  #define Capteur_GPS_h
  #include <Arduino.h>
  // Librairies pour le GPS
  #include <SoftwareSerial.h>
  #include <TinyGPSPlus.h>
  void GPS_Init(int GPSBaud);
  bool Obtain_GPS_Data(TinyGPSPlus gps);
  void SerialPrintUpdatedData(TinyGPSPlus gps);
#endif