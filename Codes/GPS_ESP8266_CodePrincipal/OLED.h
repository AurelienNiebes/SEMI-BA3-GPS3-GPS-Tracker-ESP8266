#ifndef OLED_h
  #define OLED_h
  #include <Arduino.h>
  // Librairies pour OLED
  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  void OLED_Init();
  void OLED_Clear();
  void OLED_Display();
  void OLED_Print(int x, int y, String text);
  void drawRotatedBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t angle);
  void OLED_PrintDistance(int x, int y, double distance);
  void OLED_DrawJauge(int xmin, int largeur, double distance);
#endif