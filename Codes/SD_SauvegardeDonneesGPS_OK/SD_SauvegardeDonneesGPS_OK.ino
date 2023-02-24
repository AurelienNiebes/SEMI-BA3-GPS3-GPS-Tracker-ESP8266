#include <SD.h> //used for the microSD card functionality
#include <SPI.h> //using SPI communication protocol between the Arduino Uno board and the microSD card module
#include <TinyGPS++.h>
#include <SoftwareSerial.h> 

File myFile; //to write GPS data to our .txt file in the microSD card
int RXPin = 04;
int TXPin = 05;
const int chipSelect = 15;
String filename = "GPS_data.txt";

TinyGPSPlus gps;
SoftwareSerial SerialGPS(RXPin, TXPin);

String Latitude, Longitude, Altitude, day, month, year, hour, minute, second, Date, Time;
 
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 


  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) //it will start the SPI communication using the default SPI CS pin that is GPIO15
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  }
 else Serial.println("initialization done.");
  Serial.println("Creating GPS_data.txt...");
  myFile = SD.open(filename, FILE_WRITE); //open the GPS_data.txt file on the microSD card using SD.open() and will act as read/write. If the file does not exist, it will get created.
 
  if (myFile) {
    myFile.println( "Latitude, Longitude, Altitude, Time \r\n");
    myFile.close();

  } 
  else {
    Serial.println("error opening GPS_data.txt");
  }
  
 SerialGPS.begin(9600); //open the serial communication for the GPS port at a baud rate of 9600
}

void loop() {
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
      obtain_data();
      WriteData();
      delay(10000);
  if (millis() > 5000 && gps.charsProcessed() < 10) //if there is no GPS data detected after 5s then the serial monitor will display “GPS NOT DETECTED!” message.
  {
    Serial.println("GPS NOT DETECTED!");
    while(true);
  }

}

void obtain_data()//gets the latitude, longitude, altitude, date and current time for a valid location and saves them in their appropriate variables
{
  if (gps.location.isValid())
  {
    Latitude = gps.location.lat();
    Longitude = gps.location.lng();
    Altitude = gps.altitude.meters();
  }
  else
  {
    Serial.println("Location is not available");
    Latitude=Longitude=Altitude="";
  }
  
  if (gps.date.isValid())
  {
    month = gps.date.month();
    day = gps.date.day();
    year = gps.date.year();
    Date = year + "-" + month + "-" + day;
     Serial.println(Date);
  }
  else
  {
    Serial.print("Date Not Available");
    Date="";
  }
 
  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    Serial.println(gps.time.hour());
    Serial.println(gps.time.minute());
    Serial.println(gps.time.second());

    if (gps.time.hour() < 10) ;
    hour = gps.time.hour();
    if (gps.time.minute() < 10);
    minute = gps.time.minute();
    if (gps.time.second() < 10) ;
    second = gps.time.second();
    Time = hour + ":" + minute + ":" + second;
  Serial.println(Time);
  }
  else
  {
    Serial.print("Time Not Available");
    Time="";
  }
  
}
void WriteData(){
  String Data = Latitude + "," + Longitude + "," + Altitude + "," + Date + " " + Time; //values are then linked together and saved in a string variable named ‘Data.’
  Serial.println(Data);
  myFile = SD.open(filename, FILE_WRITE);
  
  if (myFile) {
    Serial.print("GPS logging to GPS_data.txt...");
    myFile.println(Data); // pass the ‘Data’ variable inside it to be written on the .txt file
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening GPS_data.txt");
  } 
 Serial.println();
}
