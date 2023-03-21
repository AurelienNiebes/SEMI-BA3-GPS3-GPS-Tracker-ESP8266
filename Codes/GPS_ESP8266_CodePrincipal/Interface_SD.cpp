#include "Interface_SD.h"

void SD_Init(int chipSelect) {
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(chipSelect)) {
    Serial.println(F("initialization failed!"));
    while (1)
      ;
  } else {
    Serial.println(F("initialization done."));
  }
}
void CreateFileWithHeader(String PathFileName, String Header){

  Serial.printf("Creating %s...",PathFileName);
  File myFile = SD.open(PathFileName, FILE_WRITE); //open the GPS_data.txt file on the microSD card using SD.open() and will act as read/write. If the file does not exist, it will get created.
 
  if (myFile) {
    Serial.println(F("Writing Header..."));
    myFile.println(Header);
  } 
  else {
    Serial.println("error opening "+PathFileName);
  }
  myFile.close();
}

//Crée le String Data qui va être écris dans "GPS_Path.txt"
String GPSFormat(TinyGPSPlus gps){
  //TODO: Quand est-ce qu'on écrit dans le fichier ? Régulièrement ou que quand de nouvelles données sont dispos(.isUpdated())

  String Latitude, Longitude, Altitude, day, month, year, hour, minute, second, Date, Time;
  String Data;
  if (gps.location.isValid()){
    Latitude = String(gps.location.lat(),6);
    Longitude = String(gps.location.lng(),6);
    Altitude = String(gps.altitude.meters(),6);
  }
  else{
    Serial.println("Location is not available");
    Latitude=Longitude=Altitude="";
  }
  
  if (gps.date.isValid()){
    month = gps.date.month();
    day = gps.date.day();
    year = gps.date.year();
    Date = year + "-" + month + "-" + day;
    Serial.print("Date: ");
    Serial.println(Date);
  }
  else{
    Serial.println("Date Not Available");
    Date="";
  }
 
  if (gps.time.isValid()){

    hour="";
    minute="";
    second="";
    
    //TODO: Prendre en compte le fuseau horaire plus proprement
    if (gps.time.hour()+1 < 10){hour = '0';}
    hour += String(gps.time.hour()+1) ;
    
    if (gps.time.minute() < 10){minute = '0';}
    minute += String(gps.time.minute());
    
    if (gps.time.second() < 10){second  = '0';}
    second += String(gps.time.second());
    
    Time = hour + ":" + minute + ":" + second;
    Serial.print("Time: ");
    Serial.println(Time);
  }
  else{
    Serial.println("Time Not Available");
    Time="";
  }
  Data = Latitude + "," + Longitude + "," + Altitude + "," + Date + " " + Time;
  Serial.println("Data: "+Data);
  return Data;  
}

void WritePath(String Data, String PathFileName) {
  //String Latitude, String Longitude, String Altitude, String Date, String Time
  //String Data = Latitude + "," + Longitude + "," + Altitude + "," + Date + " " + Time; //values are then linked together and saved in a string variable named ‘Data.’

  File PathFile = SD.open(PathFileName, FILE_WRITE);  //Fichier où le chemin parcouru est enregistré

  if (PathFile) {
    Serial.print(F("GPS logging to "));
    Serial.println(PathFileName);
    Serial.println(Data);
    PathFile.println(Data);  // pass the ‘Data’ variable inside it to be written on the .txt file
    Serial.println(F("done."));
  } else {
    Serial.print(F("error opening "));
    Serial.println(PathFileName);
  }
  PathFile.close();
  Serial.println();
}
String ReadWaypoints(String WaypointsFileName) {
  //TODO: A compléter et tester
  //Latitude, Longitude, Nom, (Description)
  static int offset = 0;
  File WaypointsFile = SD.open(WaypointsFileName, FILE_READ);  //Fichier avec les étapes à atteindre
  String buffer;
  if (WaypointsFile) {
    Serial.println("Reading next line of " + WaypointsFileName);
    WaypointsFile.seek(offset);
    if (WaypointsFile.available()) {  //TODO: Vérifier si il n'y a aucun problème avec la dernière ligne
      buffer = WaypointsFile.readStringUntil('\n');
      Serial.println(buffer);
      offset = WaypointsFile.position();
    } else {
      //TODO: indiquer que le fichier est vide
      Serial.println(F("Dernière étape atteinte !"));
      buffer = "Fini";
    }
    WaypointsFile.close();
  } else {
    Serial.println("error opening " + WaypointsFileName);
    buffer = "Erreur";
  }
  return buffer;
}