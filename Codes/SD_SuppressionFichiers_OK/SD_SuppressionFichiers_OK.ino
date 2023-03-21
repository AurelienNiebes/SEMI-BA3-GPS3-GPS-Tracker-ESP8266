#include <SPI.h>
#include <SD.h>
File myFile;
String buffer;
String filename = "GPS_data";

const int chipSelect = 15;
const int BaudRate =115200;

void Serial_init(int BaudRate){
  Serial.begin(BaudRate);
  while (!Serial) {
  }
  Serial.println("Communication serie initialisee");
}

void SD_Init(int chipSelect) {
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.println("initialization done.");
}

void setup() {
  Serial_init(BaudRate);
  SD_Init(chipSelect);
  //Delete files

  Serial.println("Removing " + filename + ".txt");
  SD.remove(filename + ".txt");

  if (SD.exists(filename + ".txt")) {
    Serial.println(filename + ".txt exists.");
  } else {
    Serial.println(filename + ".txt doesn't exist.");
  }
}

void loop() {}
