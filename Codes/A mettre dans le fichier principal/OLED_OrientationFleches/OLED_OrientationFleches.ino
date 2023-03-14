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
static const double TRESOR_LAT = 50.45405, TRESOR_LON = 3.949944;  //Coordonnées du trésor(ici, le Beffroi de Mons)

void DrawArrow(double orientation){
  //Dessine une flèche(triangle) selon son orientation en degrés de 0 à 360
  //Fonctionne par arcs de cercle pour l'instant
  //Changer si on trouve comment tourner les flèches librement
  
  switch((int)orientation)
      {
      case 0 ... 45:
      case 315 ... 360:
        // Draw triangle (vers le haut)
        display.drawTriangle(50, 54, 78, 54, 64, 10, WHITE);
        // Fill triangle
        display.fillTriangle(50, 54, 78, 54, 64, 10, WHITE); 
        break; 
      case 46 ... 134:
      // Draw triangle (vers la droite)
        display.drawTriangle(40, 45, 40, 19, 94, 32, WHITE);
        // Fill triangle
        display.fillTriangle(40, 45, 40, 19, 94, 32, WHITE);
         break; 
      case 135 ... 225:
        // Draw triangle (vers le bas)
        display.drawTriangle(50, 10, 78, 10, 64, 54, WHITE);
        // Fill triangle
        display.fillTriangle(50, 10, 78, 10, 64, 54, WHITE);
        break; 
      case 226 ... 314:
      // Draw triangle (vers la gauche)
        display.drawTriangle(88, 45, 88, 19, 34, 32, WHITE);
        // Fill triangle
        display.fillTriangle(88, 45, 88, 19, 34, 32, WHITE);
        break; 
      default: Serial.println(F("Erreur: Orientation non valide(>360?)"));

      }
}

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
void test()
{
  for (double i=0; i<=360; i++) {
    Serial.println(i);
    display.clearDisplay();
    DrawArrow(i);
    display.display();
    delay(30);
  }
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

      Serial.print(F("TRESOR     Distance="));
      Serial.print(distanceToTRESOR/1000, 6);
      Serial.print(F(" km Course-to="));
      Serial.print(courseToTRESOR, 6);
      Serial.print(F(" degrees ["));
      Serial.print(TinyGPSPlus::cardinal(courseToTRESOR)); //NW, etc.
      Serial.println(F("]"));

       
      display.clearDisplay();
      DrawArrow(courseToTRESOR);
      display.display();
      

      if (gps.charsProcessed() < 10)
        Serial.println(F("WARNING: No GPS data.  Check wiring."));
  
      last = millis();
      Serial.println();
  }
}
}
