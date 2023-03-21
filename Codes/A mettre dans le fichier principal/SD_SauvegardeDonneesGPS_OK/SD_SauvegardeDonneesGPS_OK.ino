#include <SD.h> //used for the microSD card functionality
#include <SPI.h> //using SPI communication protocol between the Arduino Uno board and the microSD card module
#include <TinyGPS++.h>
#include <SoftwareSerial.h> 

const int RXPin = D2;
const int TXPin = D1;
const int chipSelect = D8;


String filename = "GPS_data.txt";

File myFile; //to write GPS data to our .txt file in the microSD card
TinyGPSPlus gps;
SoftwareSerial SerialGPS(RXPin, TXPin);

String Latitude, Longitude, Altitude, day, month, year, hour, minute, second, Date, Time;
bool CoordDispo =false;

void Serial_init(int BaudRate){
  Serial.begin(BaudRate);
  while (!Serial) {
  }
  Serial.println("Communication serie initialisee");
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial_init(BaudRate);
 
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)){ //it will start the SPI communication using the default SPI CS pin that is GPIO15
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  }
  else Serial.println("initialization done.");

  bool FileExisted=SD.exists(filename);
  if (!FileExisted) {
    Serial.printf("Creating %s...",filename);
  }
  myFile = SD.open(filename, FILE_WRITE); //open the GPS_data.txt file on the microSD card using SD.open() and will act as read/write. If the file does not exist, it will get created.
 
  if (myFile) {
    if(!FileExisted){
      Serial.println(F("Writing Header..."));
      myFile.println( "Latitude, Longitude, Altitude, Time \r\n");
    }
  } 
  else {
    Serial.println("error opening "+filename);
  }
  myFile.close();
  SerialGPS.begin(9600); //open the serial communication for the GPS port at a baud rate of 9600
}

void loop() {
  while (SerialGPS.available() > 0){
    if (gps.encode(SerialGPS.read())){
      obtain_data();
      if(CoordDispo){
        WriteData();      
      }
      delay(5000);
    }
  }
  if (millis() > 5000 && gps.charsProcessed() < 10) //if there is no GPS data detected after 5s then the serial monitor will display “GPS NOT DETECTED!” message.
  {
    Serial.println("GPS NOT DETECTED!");
    while(true);
  }

}

void obtain_data()//gets the latitude, longitude, altitude, date and current time for a valid location and saves them in their appropriate variables
{
  CoordDispo=gps.location.isValid();
  if (CoordDispo){
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
    /*Serial.println(gps.time.hour()+1);
    Serial.println(gps.time.minute());
    Serial.println(gps.time.second());*/

    hour="";
    minute="";
    second="";
    
    if (gps.time.hour()+1 < 10) {
    hour = '0';}
    hour += String(gps.time.hour()+1) ;
    
    if (gps.time.minute() < 10){
    minute = '0';}
    minute += String(gps.time.minute());
    
    if (gps.time.second() < 10){
    second  = '0';}
    second += String(gps.time.second());
    
    Time = hour + ":" + minute + ":" + second;
    Serial.print("Time: ");
    Serial.println(Time);
  }
  else{
    Serial.println("Time Not Available");
    Time="";
  }
  
}
void WriteData(){
  String Data = Latitude + "," + Longitude + "," + Altitude + "," + Date + " " + Time; //values are then linked together and saved in a string variable named ‘Data.’
  myFile = SD.open(filename, FILE_WRITE);
  
  if (myFile) {
    Serial.println("GPS logging to GPS_data.txt...");
    Serial.println(Data);
    myFile.println(Data); // pass the ‘Data’ variable inside it to be written on the .txt file
    Serial.println("done.");
  } else {
    Serial.println("error opening GPS_data.txt");
  }
  myFile.close(); 
  Serial.println();
}
