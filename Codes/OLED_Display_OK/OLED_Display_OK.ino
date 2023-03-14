#include <SPI.h> //utile?
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Logo localisation
// static const uint8_t image_data_Saraarray[] = {};
      
void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

// Clear the buffer
  display.clearDisplay();
  
// Logo
 // display.drawBitmap(0, 0, image_data_Saraarray, 128, 64, 1);
  // display.display();
}

void loop() {
  // Draw triangle (vers la droite)
  display.clearDisplay();
  display.drawTriangle(40, 45, 40, 19, 94, 32, WHITE);
// Fill triangle
  display.fillTriangle(40, 45, 40, 19, 94, 32, WHITE);
  display.display();
  delay(5000);

// Draw triangle (vers la gauche)
  display.clearDisplay();
  display.drawTriangle(88, 45, 88, 19, 34, 32, WHITE);
// Fill triangle
  display.fillTriangle(88, 45, 88, 19, 34, 32, WHITE);
  display.display();
  delay(5000);


// Draw triangle (vers le bas)
  display.clearDisplay();
  display.drawTriangle(50, 10, 78, 10, 64, 54, WHITE);
// Fill triangle
  display.fillTriangle(50, 10, 78, 10, 64, 54, WHITE);
  display.display();
  delay(5000);

// Draw triangle (vers le haut)
  display.clearDisplay();
  display.drawTriangle(50, 54, 78, 54, 64, 10, WHITE);
// Fill triangle
  display.fillTriangle(50, 54, 78, 54, 64, 10, WHITE);
  display.display();
  delay(5000);
}
