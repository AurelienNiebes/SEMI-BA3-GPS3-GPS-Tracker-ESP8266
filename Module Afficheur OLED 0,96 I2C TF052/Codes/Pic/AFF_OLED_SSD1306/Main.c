/****************************************************************************
 Interfacing PIC18F4550 microcontroller with SSD1306 OLED (128x64 Pixel)
 C Code for CCS C compiler
 Internal oscillator used @ 8MHz
 http://simple-circuit.com/
 
*****************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers
 
  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98
 
 This example is for a 128x64 size display using I2C to communicate
 3 pins are required to interface (2 I2C and one reset)
 
 Adafruit invests time and resources providing this open source code, 
 please support Adafruit and open-source hardware by purchasing 
 products from Adafruit!
 
 Written by Limor Fried/Ladyada  for Adafruit Industries.  
 BSD license, check license.txt for more information
 All text above, and the splash screen must be included in any redistribution
*****************************************************************************/

// ATTENTION Build configuration doit etre sur Release
// sinon erreur de manque de memoire en RAM
 
// SSD1306 OLED reset pin definition
#define SSD1306_RST    PIN_B2
 
#include <18F4550.h>
#device PASS_STRINGS = IN_RAM
#fuses HS,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,VREGEN
#use delay(clock=24M)
#use fast_io(B)
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Initialize I2C
 
// Include SSD1306 OLED driver source code
#include <SSD1306OLED.c>
 
void testdrawcircle(void) {
  for (int8 i = 0; i < SSD1306_LCDHEIGHT; i += 2) {
    SSD1306_DrawCircle(SSD1306_LCDWIDTH/2, SSD1306_LCDHEIGHT/2, i);
    SSD1306_Display();
    delay_ms(1);
  }
}
 
void testfillrect(void) {
  int1 color = TRUE;
  for (int8 i = 0; i < SSD1306_LCDHEIGHT/2; i += 3) {
    // alternate colors
    SSD1306_FillRect(i, i, SSD1306_LCDWIDTH - i*2, SSD1306_LCDHEIGHT - i*2, color);
    SSD1306_Display();
    delay_ms(1);
    if(color)  color = FALSE;
    else       color = TRUE;
  }
}
 
void testdrawtriangle(void) {
  for (int8 i = 0; i < SSD1306_LCDHEIGHT/2; i += 5) {
    SSD1306_DrawTriangle(SSD1306_LCDWIDTH/2,     SSD1306_LCDHEIGHT/2 - i,
                         SSD1306_LCDWIDTH/2 - i, SSD1306_LCDHEIGHT/2 + i,
                         SSD1306_LCDWIDTH/2 + i, SSD1306_LCDHEIGHT/2 + i);
    SSD1306_Display();
    delay_ms(1);
  }
}
 
void testfilltriangle(void) {
  int1 color = TRUE;
  for (signed int16 i = SSD1306_LCDHEIGHT/2; i > 0; i -= 5) {
    SSD1306_FillTriangle(SSD1306_LCDWIDTH/2,     SSD1306_LCDHEIGHT/2 - i,
                         SSD1306_LCDWIDTH/2 - i, SSD1306_LCDHEIGHT/2 + i,
                         SSD1306_LCDWIDTH/2 + i, SSD1306_LCDHEIGHT/2 + i, color);
    if(color)  color = FALSE;
    else       color = TRUE;
    SSD1306_Display();
    delay_ms(1);
  }
}
 
void testdrawroundrect(void) {
  for (int8 i = 0; i < SSD1306_LCDHEIGHT/2 - 2; i += 2) {
    SSD1306_DrawRoundRect(i, i, SSD1306_LCDWIDTH - 2*i, SSD1306_LCDHEIGHT - 2*i, SSD1306_LCDHEIGHT/4 - i/2);
    SSD1306_Display();
    delay_ms(1);
  }
}
 
void testfillroundrect(void) {
  int1 color = TRUE;
  for (int8 i = 0; i < SSD1306_LCDHEIGHT/2 - 2; i += 2) {
    SSD1306_FillRoundRect(i, i, SSD1306_LCDWIDTH - 2*i, SSD1306_LCDHEIGHT - 2*i, SSD1306_LCDHEIGHT/4 - i/2, color);
    if(color)  color = FALSE;
    else       color = TRUE;
    SSD1306_Display();
    delay_ms(1);
  }
}
   
void testdrawrect(void) {
  for (int8 i = 0; i < SSD1306_LCDHEIGHT/2; i += 2) {
    SSD1306_DrawRect(i, i, SSD1306_LCDWIDTH - 2*i, SSD1306_LCDHEIGHT - 2*i);
    SSD1306_Display();
    delay_ms(1);
  }
}
 
void testdrawline() {  
  signed int16 i;
  for (i = 0; i < SSD1306_LCDWIDTH; i += 4) {
    SSD1306_DrawLine(0, 0, i, SSD1306_LCDHEIGHT - 1);
    SSD1306_Display();
    delay_ms(1);
  }
  for (i = 0; i < SSD1306_LCDHEIGHT; i += 4) {
    SSD1306_DrawLine(0, 0, SSD1306_LCDWIDTH - 1, i);
    SSD1306_Display();
    delay_ms(1);
  }
  delay_ms(250);
  
  SSD1306_ClearDisplay();
  for (i = 0; i < SSD1306_LCDWIDTH; i += 4) {
    SSD1306_DrawLine(0, SSD1306_LCDHEIGHT - 1, i, 0);
    SSD1306_Display();
    delay_ms(1);
  }
  for (i = SSD1306_LCDHEIGHT - 1; i >= 0; i -= 4) {
    SSD1306_DrawLine(0, SSD1306_LCDHEIGHT - 1, SSD1306_LCDWIDTH - 1, i);
    SSD1306_Display();
    delay_ms(1);
  }
  delay_ms(250);
  
  SSD1306_ClearDisplay();
  for (i = SSD1306_LCDWIDTH - 1; i >= 0; i -= 4) {
    SSD1306_DrawLine(SSD1306_LCDWIDTH - 1, SSD1306_LCDHEIGHT - 1, i, 0);
    SSD1306_Display();
    delay_ms(1);
  }
  for (i = SSD1306_LCDHEIGHT - 1; i >= 0; i -= 4) {
    SSD1306_DrawLine(SSD1306_LCDWIDTH - 1, SSD1306_LCDHEIGHT - 1, 0, i);
    SSD1306_Display();
    delay_ms(1);
  }
  delay_ms(250);
 
  SSD1306_ClearDisplay();
  for (i = 0; i < SSD1306_LCDHEIGHT; i += 4) {
    SSD1306_DrawLine(SSD1306_LCDWIDTH - 1, 0, 0, i);
    SSD1306_Display();
    delay_ms(1);
  }
  for (i = 0; i < SSD1306_LCDWIDTH; i += 4) {
    SSD1306_DrawLine(SSD1306_LCDWIDTH - 1, 0, i, SSD1306_LCDHEIGHT - 1); 
    SSD1306_Display();
    delay_ms(1);
  }
  delay_ms(250);
}
 
void testscrolltext(void) {
  SSD1306_ClearDisplay();
  SSD1306_DrawText(58, 8, "scroll", 2);
  //SSD1306_DrawText(58, 8, *str1, 2);
  SSD1306_Display();
  delay_ms(1);
 
  SSD1306_StartScrollRight(0x00, 0x0F);
  delay_ms(2000);
  SSD1306_StopScroll();
  delay_ms(1000);
  SSD1306_StartScrollLeft(0x00, 0x0F);
  delay_ms(2000);
  SSD1306_StopScroll();
  delay_ms(1000);    
  SSD1306_StartScrollDiagRight(0x00, 0x07);
  delay_ms(2000);
  SSD1306_StartScrollDiagLeft(0x00, 0x07);
  delay_ms(2000);
  SSD1306_StopScroll();
}
 
 
void main(void) {
 
  //setup_oscillator(OSC_8MHZ);                // Set internal oscillator to 8MHz
 
  delay_ms(1000);
 
  // Initialize the SSD1306 OLED with an I2C addr = 0x7A (default address)
  SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
 
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  SSD1306_Display();
  delay_ms(2000);
 
  // Clear the buffer.
  SSD1306_ClearDisplay();
 
  // draw a single pixel
  SSD1306_DrawPixel(10, 10);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call SSD1306_Display() after making any drawing commands
  // to make them visible on the display hardware!
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  // draw many lines
  testdrawline();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  // draw rectangles
  testdrawrect();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  // draw multiple rectangles
  testfillrect();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  // draw mulitple circles
  testdrawcircle();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  // draw a white circle, 10 pixel radius
  SSD1306_FillCircle(SSD1306_LCDWIDTH/2, SSD1306_LCDHEIGHT/2, 10);
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  testdrawroundrect();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  testfillroundrect();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  testdrawtriangle();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  testfilltriangle();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  // draw scrolling text
  testscrolltext();
  SSD1306_Display();
  delay_ms(2000);
  SSD1306_ClearDisplay();
 
  // text display tests
  char txt[9];
  SSD1306_DrawText(2, 7, "Hello, world!", 1);
  //SSD1306_DrawText(2, 7, *str2, 1);
  
  sprintf(txt, "%.6f", 3.141592);
  SSD1306_DrawText(2, 16, txt, 1);
  
  SSD1306_DrawText(2, 26, "0x", 2);
  //SSD1306_DrawText(2, 26, *str3, 2);
  
  sprintf(txt, "%LX", 0xDEADBEEF);
  SSD1306_DrawText(26, 26, txt, 2);
  SSD1306_Display();
  delay_ms(2000);
  
  while(TRUE) {
    // invert the display
    SSD1306_InvertDisplay(TRUE);
    delay_ms(1000);
    SSD1306_InvertDisplay(FALSE);
    delay_ms(1000);
  }
 
}
// End of code.