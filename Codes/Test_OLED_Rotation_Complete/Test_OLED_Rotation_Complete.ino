//Volé de bodmer sur https://forum.arduino.cc/t/rotating-a-bitmap/404329/21
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// These are the pins I use on a NodeMCU ESP-12E
//#define OLED_SDA D7 //MOSI
//#define OLED_SCL D5 //CLK
//#define OLED_DC  D4  //DC
//#define OLED_CS  12  // My OLED does no have a CS line, so just use a spare pin to keep library happy
//#define OLED_RST D3 //RES

// Software SPI on NodeMCU
//Adafruit_SSD1306 display(OLED_SDA, OLED_SCL, OLED_DC, OLED_RST, OLED_CS);

// Don't specify MOSI and SCLK so hardware SPI on NodeMCU
//Adafruit_SSD1306 display(OLED_DC, OLED_RST, OLED_CS);


// Using hardware SPI pins:
/*#define OLED_DIN   12
#define OLED_CLK   13
#define OLED_DC    10
#define OLED_CS    9
#define OLED_RESET -1*/

// Software bit bashed SPI
//Adafruit_SSD1306 display(OLED_DIN, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Hardware SPI
//Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


#define DEG2RAD 0.0174532925

long unsigned int startMillis;
short unsigned int iter = 0;              // used to calculate the frames per second (FPS)
int angle = 0;

const static uint8_t PROGMEM testbmp1[] =
{ 16, 8,
  B00000001, B10000000,
  B00000010, B01000000,
  B00000100, B00100000,
  B00001000, B00010000,
  B00010000, B00001000,
  B00100000, B00000100,
  B01000000, B00000010,
  B10000000, B00000001
};

const static uint8_t PROGMEM flechebmp[] =
{ 16, 9,
  B00000011, B00000000,
  B00001111, B00000000,
  B00111111, B00000000,
  B01111111, B11111111,
  B11111111, B11111111,
  B01111111, B11111111,
  B00111111, B00000000,
  B00001111, B00000000,
  B00000011, B00000000
};
const PROGMEM static uint8_t epd_bitmap_Fleche [] = {
  40, 56,
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 
	0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x03, 
	0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 
	0xf0, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xfc, 
	0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 
	0x01, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xc0, 0x07, 
	0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 
	0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 
	0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 
	0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 
	0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 
	0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 
	0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 
	0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 
	0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 
	0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 
	0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00
};

const static uint8_t PROGMEM bmp5x5[] =
{ 5, 5,
  B11111100, B01101011,
  B00011111, B10000000,
};

static const uint8_t PROGMEM adafruit[] =
{ 16, 16,
  B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  Serial.begin(115200);
  // Need to hardware reset my display manually via pin 7
  /*digitalWrite(7, LOW);
  pinMode(7, OUTPUT);
  delay(200);
  pinMode(7, INPUT);
  delay(5);*/
  
  // by default, we'll generate the high voltage from the 3.3v line
  //display.begin(SSD1306_SWITCHCAPVCC);  // initialize
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));//Le F veut dire que le string est stockée dans la mémoire Flash
    for(;;);
  }
}

void loop() {
  int t0=millis();
  for (int angle = 0; angle < 360; angle++) {
    display.clearDisplay(); // Clear the display buffer

    drawRotatedBitmap(0, 0, epd_bitmap_Fleche, angle);
    //drawRotatedBitmap(64, 16, adafruit, angle);
    //drawRotatedBitmap(1, 1, bmp5x5, angle);
    display.display();     // Now update the display with the buffer
    //delay(100); // Pause so we see it
  }
  Serial.printf("Temps pris: %u\r\n",millis()-t0);
}

void drawRotatedBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t angle) {
  //x, y: Les coordonées du centre de l'image ?
  uint8_t w = pgm_read_byte(bitmap++);//Lit le premier nombre du bitmap(la largeur)
  uint8_t h = pgm_read_byte(bitmap++);//Lit le second nombre du bitmap(la largeur)

  int16_t newx, newy;
  uint8_t data = 0;

  float  cosa = cos(angle * DEG2RAD), sina = sin(angle * DEG2RAD);

  //Coordonnées après rotation du bord le plus proche de l'origine de l'image
  x = x - ((w * cosa / 2) - (h * sina / 2));
  y = y - ((h * cosa / 2) + (w * sina / 2));

  for (int16_t j = 0; j < h; j++) {
    for (int16_t i = 0; i < w; i++ ) {
      if ((j * w + i) & 7) data <<= 1;//Data shifté vers la gauche de 1
      else      data   = pgm_read_byte(bitmap++);//Tous les 8 pixels, data vaut l'octet suivant du bitmap

      newx = 0.5 + x + ((i * cosa) - (j * sina));//Pq le 0.5 ?
      newy = 0.5 + y + ((j * cosa) + (i * sina));

      if (data & 0x80) display.drawPixel(newx, newy, 1);//Si le bit le plus fort de data vaut 1, dessine un pixel
      //else            display.drawPixel(newx, newy, 0);
    }
  }
}