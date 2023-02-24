//Libraries
#include <SD.h>//https://www.arduino.cc/en/reference/SD
#include <SPI.h>//https://www.arduino.cc/en/Reference/SPI
//Constants
#define SD_ChipSelectPin 15
//Objects
File root;
void setup() {
 	//Init Serial USB
 	Serial.begin(9600);
 	Serial.println(F("Initialize System"));
 	//Init sd shield
 	if (!SD.begin(SD_ChipSelectPin)) {
 			Serial.println(F("SD fail"));
 			return;
 	}
 	printFilenames();
}
void loop() {
}
void printFilenames(void ) { /* function printFilenames */
 	////find files in SD card
 	root = SD.open("/");
 	while (true) {
 			File entry = 	root.openNextFile();
 			if (! entry) {
        Serial.println("no more file");
 					break;// no more files
 			}
 			Serial.println(entry.name());
 			entry.close();
 	}
 	root.close();
}
