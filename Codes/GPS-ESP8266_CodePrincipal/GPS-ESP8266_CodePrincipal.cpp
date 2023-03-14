#include "GPS-ESP8266_CodePrincipal.h"

// Librairies pour le GPS

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <TinyGPSPlus.h>

 // Librairies pour OLED
 
#include <SPI.h>  
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SD.h> //used for the microSD card functionality
#include <SPI.h> //using SPI communication protocol between the Arduino Uno board and the microSD card module
