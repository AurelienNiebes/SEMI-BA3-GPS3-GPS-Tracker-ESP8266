//Essaye de récupérer la vitesse des phrases NMEA $GPRMC, $GPVTG
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

const int RXPin = D4, TXPin = D3;  //D4 bleu et D3 vert
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial SerialGPS(RXPin, TXPin);

TinyGPSCustom VitKnot(gps, "GPRMC", 7); // $GPRMC sentence, 7th element
TinyGPSCustom Direction(gps, "GPRMC", 8); // $GPRMC sentence, 8th element
TinyGPSCustom VNordDirection(gps, "GPVTG", 1); // $GPVTG sentence, 1st element
TinyGPSCustom MNordDirection(gps, "GPVTG", 3); // $GPVTG sentence, 3th element
TinyGPSCustom VitKMH(gps, "GPVTG", 7); // $GPVTG sentence, 7th element

bool Obtain_GPS_Data(TinyGPSPlus gps){
  static int last=millis();
  while (SerialGPS.available() > 0){
    return gps.encode(SerialGPS.read());
  }
  if ((last - millis()) > 5000 && gps.charsProcessed() < 10) //if there is no GPS data detected after 5s then the serial monitor will display “GPS NOT DETECTED!” message.
  {
    Serial.println(F("GPS NOT DETECTED!"));
    while(true);
  }
  return false;
}
void GPS_Init(int GPSBaud){
  SerialGPS.begin(GPSBaud);
  Serial.println(F("GPS connecté"));
}
void setup() 
{
  Serial.begin(115200);
  GPS_Init(GPSBaud);

  Serial.println(F("Test_GPS_Vitesse.ino"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop() 
{
  
  // Every time anything is updated, print everything.
  if (Obtain_GPS_Data(gps))
  {
    Serial.print(F("VIT(KNOT)="));   Serial.print(VitKnot.value()); 
    Serial.print(F(" VIT(KMH)=")); Serial.print(VitKMH.value()); 
    Serial.print(F(" Direction=")); Serial.print(Direction.value()); 
    Serial.print(F(" VDirection=")); Serial.print(VNordDirection.value());
    Serial.print(F(" MDirection=")); Serial.println(MNordDirection.value());
  }
}

