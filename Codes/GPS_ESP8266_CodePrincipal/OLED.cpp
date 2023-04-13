#include "OLED.h"
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
void OLED_Init() {
  Serial.println(F("Initialisation de l\'OLED"));
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64 OLED Screen
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  } else {
    display.clearDisplay();
  }
}
void OLED_Clear(){display.clearDisplay();}
void OLED_Display(){display.display();}
void OLED_PrintDistance(double distance) {
  static double PrecDistance = -1;
  if (PrecDistance > 0) {
    display.setCursor(0, 20);
    if (PrecDistance > distance) {
      Serial.println(F("Vous chauffez"));
      display.println("Vous chauffez");
    } else if (PrecDistance < distance) {
      Serial.println(F("Vous vous éloignez"));
      display.println("Vous vous eloignez");
    } else {
      Serial.println(F("Bougez svp"));
      display.println("Bougez svp");
    }
  }
  PrecDistance = distance;
}
#define DEG2RAD 0.0174532925
void drawRotatedBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t angle) {
  //x, y: Les coordonées du centre de l'image ?
  uint8_t w = pgm_read_byte(bitmap++);  //Lit le premier nombre du bitmap(la largeur)
  uint8_t h = pgm_read_byte(bitmap++);  //Lit le second nombre du bitmap(la largeur)

  int16_t newx, newy;
  uint8_t data = 0;

  float cosa = cos(angle * DEG2RAD), sina = sin(angle * DEG2RAD);

  //Coordonnées après rotation du bord le plus proche de l'origine de l'image
  x = x - ((w * cosa / 2) - (h * sina / 2));
  y = y - ((h * cosa / 2) + (w * sina / 2));

  for (int16_t j = 0; j < h; j++) {
    for (int16_t i = 0; i < w; i++) {
      if ((j * w + i) & 7) data <<= 1;      //Data shifté vers la gauche de 1
      else data = pgm_read_byte(bitmap++);  //Tous les 8 pixels, data vaut l'octet suivant du bitmap

      newx = 0.5 + x + ((i * cosa) - (j * sina));  //Pq le 0.5 ?
      newy = 0.5 + y + ((j * cosa) + (i * sina));

      if (data & 0x80) display.drawPixel(newx, newy, 1);  //Si le bit le plus fort de data vaut 1, dessine un pixel
      //else            display.drawPixel(newx, newy, 0);
    }
  }
}