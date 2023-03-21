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
 	
 	//Read file line by line
  
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
}
void loop() { }
