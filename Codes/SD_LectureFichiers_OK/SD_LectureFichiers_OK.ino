#include <SPI.h>
#include <SD.h>
File myFile;
String buffer;
String filename = "GPS_Waypoints";

const int chipSelect = D8;
const int BaudRate = 9600;

void Serial_init(int BaudRate) {
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
  /*Serial.begin(BaudRate);
  while (!Serial) {
  }
  Serial.println("Communication serie initialisee");*/
  
  Serial.print("Debut !");
  delay(3000);
  Serial.print("Attention !");
  delay(3000);
  SD_Init(chipSelect);
  /*Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while(true);
    return;
  }

  Serial.println("initialization done.");*/

}
void loop() {  
  myFile = SD.open(filename + ".txt");
  Serial.println(filename + ".txt :");
  if (myFile) {
    Serial.println("Read " + filename + ".txt line by line:");
    while (myFile.available()) {
      buffer = myFile.readStringUntil('\n');
      Serial.println(buffer);
    }
    myFile.close();
  } else {
    Serial.println("error opening " + filename + ".txt");
  }
  delay(1000);
}
