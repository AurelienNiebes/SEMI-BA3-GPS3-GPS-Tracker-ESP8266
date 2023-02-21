#include <SPI.h>
#include <SD.h>
File myFile;
String buffer;
String filename = "test";
void setup() {
 	Serial.begin(9600);
 	while (!Serial) {
 	}
 	Serial.print("Initializing SD card...");
 	if (!SD.begin(4)) {
 			Serial.println("initialization failed!");
 			return;
 	}
 	Serial.println("initialization done.");
 	myFile = SD.open(filename + ".txt", FILE_WRITE);
 	if (myFile) {
 			Serial.print("Writing to " + filename + ".txt...");
 			myFile.println("testing 1, 2, 3.");
 			myFile.close();
 			Serial.println("done.");
 	} else {
 			Serial.println("error opening " + filename + ".txt");
 	}
 	//Read file byte by byte
 	myFile = SD.open(filename + ".txt");
 	if (myFile) {
 			Serial.println("Read " + filename + ".txt byte by byte:");
 			while (myFile.available()) {
 					Serial.write(myFile.read());
 			}
 			myFile.close();
 	} else {
 			Serial.println("error opening " + filename + ".txt");
 	}
 	//Read file line by line
 	myFile = SD.open(filename + ".txt");
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