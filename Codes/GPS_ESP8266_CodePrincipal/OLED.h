#ifndef OLED_h
  #define OLED_h
  #include <Arduino.h>
  // Librairies pour OLED
  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  void OLED_Init(Adafruit_SSD1306 display);
  void drawRotatedBitmap(Adafruit_SSD1306 display, int16_t x, int16_t y, const uint8_t *bitmap, uint16_t angle);
#endif