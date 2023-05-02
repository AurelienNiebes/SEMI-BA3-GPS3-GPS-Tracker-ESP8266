////////////////////////////////////////////////////////////////////////////
////                                                                    ////
////                 Driver for common LCD modules                      ////
////                                                                    ////
////  lcd_init()   Must be called before any other function.            ////
////                                                                    ////
////  lcd_putc(c)  Will display c on the next position of the LCD.      ////
////                     The following have special meaning:            ////
////                      \f  Clear display                             ////
////                      \n  Go to start of second line                ////
////                      \b  Move back one position                    ////
////                                                                    ////
////  lcd_gotoxy(x,y) Set write position on LCD (upper left is 1,1)     ////
////                                                                    ////
////                                                                    ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////                                                                    ////
//// This source code may only be used by licensed users of the CCS C   ////
//// compiler.  This source code may only be distributed to other       ////
//// licensed users of the CCS C compiler.  No other use, reproduction  ////
//// or distribution is permitted without written permission.           ////
//// Derivative programs created using this software in object code     ////
//// form are not restricted in any way.                                ////
////////////////////////////////////////////////////////////////////////////
// As defined in the following structure the pin connection is as follows:
//     D0  not use
//     D1  not use
//     D2  rs
//     D3  enable
//     D4  D4
//     D5  D5
//     D6  D6
//     D7  D7
//
//   LCD pins D0-D3 are not used and PIC D3 is not used.

struct lcd_pin_map {
		boolean unused1;							// Bit 7 not use for LCD
		boolean unused2;							// Bit 6 not use for LCD
		boolean rs;									// Bit 5 use for RS (LCD)
		boolean en;									// Bit 7 use for Enable LCD
		int data : 4;								// Bit 0 to 3 --> four bits LCD Bus
} lcd; 


#byte lcd = 6
								// The bus LCD is Port B
#define set_tris_lcd(x) set_tris_b(x)

struct lcd_pin_map const LCD_WRITE = {0,0,0,0,0};


byte CONST LCD_INIT_STRING[4] = {0x20,0x28,0x06,0x0d};  // Table for init LCD


void lcd_send_nibble(byte n);
void lcd_send_byte(byte cmd, byte n);
void lcd_char(char c);
void lcd_init();
void lcd_gotoxy(byte colonne, byte ligne);
void lcd_cursor(boolean x);
void lcd_blink(boolean x);
void lcd_printf(char* message);

//---------------------Send Half Byte------------------
void lcd_send_nibble(byte n) {
		lcd.data = n;								// Place byte on bus
		delay_cycles(1);							// Wait 1 cycle µP
		lcd.en = 1;									// Enable LCD is Hight
		delay_cycles(1);							// Wait 1 cycle µP
		lcd.en = 0;									// Enable LCD is Low
}

//--------------Send byte to LCD-------------------------
//  If cmd = 0 --> Command
//  If cmd = 1 --> Char
//
void lcd_send_byte(byte cmd, byte n) {
		lcd.rs = cmd;								// Rs is 0 to command & 1 to 
		lcd.en = 0;									// enable is low
		lcd_send_nibble(n >> 4);					// Rotate right 4 time & send
		lcd_send_nibble(n & 0xf);					// mask the half byte
		delay_us(39);
}

//------------------------LCD CHAR------------------------
// Send the char to the Display LCD
// Send too the special command to the LCD
void lcd_char(char c) {
	switch (c) {
		case '\f' : lcd_send_byte(0,1);				// Clear Display + Home Cursor
					delay_ms(2);	break;
//		case '\h' : lcd_send_byte(0,3);				// Juste Home Cursor
					delay_ms(2);	break;
		case '\n'   : lcd_gotoxy(1,2); break;		// Display on X & Y position
		case '\r' : lcd_send_byte(0,0x1c); break;	// Display Shift right
//		case '\l' : lcd_send_byte(0,0x18); break;	// Display Shift left
		case '\b' : lcd_send_byte(0,0x10); break;	// Cursor Shift left
		default	  : lcd_send_byte(1,c); break;		// Send char to the display
	}
}

//------------------------LCD INIT------------------------
void lcd_init() {
	byte i;
	set_tris_lcd(LCD_WRITE);						// Place PortB to Output (LCD Bus)
	for(i=0;i<=3;++i) {
		lcd_send_byte(0,LCD_INIT_STRING[i]);		// Send the Values to Init LCD
		delay_ms(2);
	}
}

//------------------------LCD position XY------------------
// X = Colonne
// Y = Line
void lcd_gotoxy(byte colonne, byte ligne) {
  if (ligne == 1) lcd_send_byte(0,0x80+0x00+colonne-1); //0x80 = instruction Set DDRAM Adresse) + 0x00(=DDRAM Adress de ligne 1) + (colonne- 1) (adresse de la colonne)
  if (ligne == 2) lcd_send_byte(0,0x80+0x40+colonne-1); //adresse ligne 2 = 0x40
  if (ligne == 3) lcd_send_byte(0,0x80+0x14+colonne-1); //adresse ligne 3 = 0x14
  if (ligne == 4) lcd_send_byte(0,0x80+0x54+colonne-1); //adresse ligne 4 = 0x54
}

//------------------LCD CURSOR  ON/OFF----------------------
// x = true --> cursor ON
// x = false --> cursor OFF
void lcd_cursor(boolean x) {
	if (x == true)  
		lcd_send_byte(0,0x0e); 						// Curseur ON
	else lcd_send_byte(0,0x0c);     				// Curseur OFF
}
//------------------------LCD CURSOR-----------------------
// x = true --> cursor ON
// x = false --> cursor OFF

void lcd_blink(boolean x) {
	if (x == true) 
		lcd_send_byte(0,0x0f); 						// Curseur ON
	else lcd_send_byte(0,0x0e);     				// Curseur OFF

}

void lcd_printf(char *message)				// Write message to LCD (C string type)
{
	while (*message)								//	Look for end of string
		lcd_send_byte(1,*message++);						//	Show and bump
}
