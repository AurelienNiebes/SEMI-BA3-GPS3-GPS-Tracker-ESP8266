#include <SPI.h>  //utile?
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


static const int RXPin = D4, TXPin = D3;  //D4 bleu et D3 vert
static const uint32_t SerialBaud = 115200;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// For stats that happen every 5 seconds
unsigned long last = 0UL;

double distanceToTRESORtemp = 0;
static const double TRESOR_LAT = 50.45405, TRESOR_LON = 3.949944;  //Coordonnées du trésor(ici, le Beffroi de Mons)

void setup() {
  Serial.begin(SerialBaud);
  ss.begin(GPSBaud);

  Serial.println(F("OLED_DistanceChaudFroid_OK.ino"));
  Serial.println(F("Calcule la distance entre un point et notre position"));
  Serial.println(F("Et l'affiche sur un ecran OLED"));
  Serial.print(F("Testing TinyGPSPlus library v. "));
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Clear the buffer
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());

  if (gps.location.isUpdated()) {
    Serial.print(F("LOCATION  Lat:"));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" Long="));
    Serial.println(gps.location.lng(), 6);
  }

  else if (gps.course.isUpdated()) {
    Serial.print(F("COURSE     "));
    Serial.print(F("Deg="));
    Serial.println(gps.course.deg());
  }

  else if (gps.altitude.isUpdated()) {
    Serial.print(F("ALTITUDE   "));
    Serial.print(F(" Meters="));
    Serial.println(gps.altitude.meters());
  }

  else if (millis() - last > 5000) {
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
      Serial.print(distanceToTRESOR / 1000, 6);
      Serial.print(F(" km Course-to="));
      Serial.print(courseToTRESOR, 6);
      Serial.print(F(" degrees ["));
      Serial.print(TinyGPSPlus::cardinal(courseToTRESOR));  //returns course in degrees (North=0, West=270) from position 1 to position 2
      Serial.println(F("]"));

      display.setCursor(0, 50);

      //règle de 3 : map()

      if (distanceToTRESORtemp > distanceToTRESOR) {
        Serial.println(F("Vous chauffez"));
        display.println("Vous chauffez");
        display.drawRect(10, 10, 50, 30, WHITE);
        display.fillRect(30, 10, 50, 30, WHITE); //rectangle à remplir selon la distance
        display.display();
        display.clearDisplay();
      }
      else if (distanceToTRESORtemp < distanceToTRESOR) {
        Serial.println(F("Vous vous éloignez"));
        display.println("Vous vous eloignez");
        display.drawRect(10, 10, 50, 30, WHITE);
        display.fillRect(30, 10, 50, 30, WHITE); //rectangle à remplir selon la distance
        display.display();
        display.clearDisplay();
      }
      else {
        Serial.println(F("Bougez svp"));
        display.println("Bougez svp");
        display.drawRect(10, 10, 50, 30, WHITE);
        display.fillRect(30, 10, 50, 30, WHITE); //rectangle à remplir selon la distance
        display.display();
        display.clearDisplay();
      }

      distanceToTRESORtemp = distanceToTRESOR;

      if (gps.charsProcessed() < 10)
        Serial.println(F("WARNING: No GPS data.  Check wiring."));

      last = millis();
      Serial.println();
    }
  }
}
