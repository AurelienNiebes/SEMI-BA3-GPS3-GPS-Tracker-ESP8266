#include <SPI.h>
#include <SD.h>
File myFile;
String buffer;
String filename = "GPS_data";
const int chipSelect = 15;

void setup() {
   Serial.begin(9600);
  while (!Serial) {
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
      Serial.println("initialization failed!");
      return;
  }

  Serial.println("initialization done.");

//Delete files

 Serial.println("Removing " + filename + ".txt");
  SD.remove(filename + ".txt");
  
  if (SD.exists(filename + ".txt")) {
    Serial.println(filename + ".txt exists.");
  } else {
    Serial.println(filename + ".txt doesn't exist.");
  }
}

  void loop() { }
