#include <SPI.h> //utile?
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


static const int RXPin = D4, TXPin = D3; //D4 bleu et D3 vert
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// For stats that happen every 5 seconds
unsigned long last = 0UL;

double courseToTRESORtemp = 0;

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("KitchenSink.ino"));
  Serial.println(F("Demonstrating nearly every feature of TinyGPSPlus"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();


  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }

// Clear the buffer
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop()
{
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());

  if (gps.location.isUpdated())
  {
    Serial.print(F("LOCATION  Lat:"));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" Long="));
    Serial.println(gps.location.lng(), 6);
  }

  else if (gps.course.isUpdated())
  {
    Serial.print(F("COURSE     "));
    Serial.print(F("Deg="));
    Serial.println(gps.course.deg());
  }

  else if (gps.altitude.isUpdated())
  {
    Serial.print(F("ALTITUDE   "));
    Serial.print(F(" Meters="));
    Serial.println(gps.altitude.meters());
  }

  else if (millis() - last > 5000)
  {
    Serial.println();
    if (gps.location.isValid()) {
      static const double TRESOR_LAT = 50.4623627, TRESOR_LON = 4.0498483;
      double distanceToTRESOR =
        TinyGPSPlus::distanceBetween(
          gps.location.lat(),
          gps.location.lng(),
          TRESOR_LAT, 
          TRESOR_LON);

      double courseToTRESOR =
        TinyGPSPlus::courseTo(
          gps.location.lat(),
          gps.location.lng(),
          TRESOR_LAT, 
          TRESOR_LON);
          
/*
double TinyGPSPlus::courseTo(double lat1, double long1, double lat2, double long2)
{
  // returns course in degrees (North=0, West=270) from position 1 to position 2,
  // both specified as signed decimal-degrees latitude and longitude.
  // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
  // Courtesy of Maarten Lamers
  double dlon = radians(long2-long1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double a1 = sin(dlon) * cos(lat2);
  double a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  return degrees(a2);
}
*/

      Serial.print(F("TRESOR     Distance="));
      Serial.print(distanceToTRESOR/1000, 6);
      Serial.print(F(" km Course-to="));
      Serial.print(courseToTRESOR, 6);
      Serial.print(F(" degrees ["));
      Serial.print(TinyGPSPlus::cardinal(courseToTRESOR)); //NW, etc.
      Serial.println(F("]"));

      display.setCursor(0, 20);
      
      if (cardinal(courseToTRESOR)) {
        // Draw triangle (vers la droite)
        display.clearDisplay();
        display.drawTriangle(40, 45, 40, 19, 94, 32, WHITE);
        // Fill triangle
        display.fillTriangle(40, 45, 40, 19, 94, 32, WHITE);
        display.display();
        delay(5000);
      }
      
      else if (cardinal(courseToTRESOR)) {
        // Draw triangle (vers la gauche)
        display.clearDisplay();
        display.drawTriangle(88, 45, 88, 19, 34, 32, WHITE);
        // Fill triangle
        display.fillTriangle(88, 45, 88, 19, 34, 32, WHITE);
        display.display();
        delay(5000);
      }
          
      else if (cardinal(courseToTRESOR)) {
        // Draw triangle (vers le bas)
        display.clearDisplay();
        display.drawTriangle(50, 10, 78, 10, 64, 54, WHITE);
        // Fill triangle
        display.fillTriangle(50, 10, 78, 10, 64, 54, WHITE);
        display.display();
        delay(5000);
      }

      else if (cardinal(courseToTRESOR)) {
        // Draw triangle (vers le haut)
        display.clearDisplay();
        display.drawTriangle(50, 54, 78, 54, 64, 10, WHITE);
        // Fill triangle
        display.fillTriangle(50, 54, 78, 54, 64, 10, WHITE);
        display.display();
        delay(5000);
      }

      else {
            Serial.println(F("Bougez svp"));
            display.println("Bougez svp");
            display.display(); 
            display.clearDisplay();
          } 
          
      courseToTRESORtemp = courseToTRESOR;

      if (gps.charsProcessed() < 10)
        Serial.println(F("WARNING: No GPS data.  Check wiring."));
  
      last = millis();
      Serial.println();
  }
}
}
