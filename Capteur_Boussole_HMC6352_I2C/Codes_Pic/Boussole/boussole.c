//-----------------------------------------------------
//------------- Programme Boussole ---------------
//-----------------------------------------------------

#include <16F876.h>

#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#use i2c(master, sda=PIN_C4, scl=PIN_C3)

//-------------------------E/S------------------------
#include "Define.h"
#include "db_lcd.c"

Byte CONST boussole = 0b01000010; // Adresse Boussole [HMC6352] (0x42) 

//-------------------------Var------------------------


//-------------------------xxx------------------------


//-------------Lecture  I2C  1 byte------------------
signed byte lec_i2c(byte device, byte address) {
   signed BYTE data;

   i2c_start();
   i2c_write(device);
   i2c_write(address);
   i2c_start();
   i2c_write(device | 1);
   data=i2c_read(0);
   i2c_stop();
   return(data);
}

//-------------Lecture  I2C  2 bytes -----------------
signed int16 lecdb_i2c(byte device, byte address) {
   BYTE dataM,dataL;
   int16 data;

   i2c_start();
   i2c_write(device);
   i2c_write(address);
   i2c_start();
   i2c_write(device | 1);
   dataM = i2c_read(1);				// Lecteur du MSB
   dataL = i2c_read(0);				// Lecteur du LSB
   i2c_stop();
   data=((dataM*256)+dataL);		// recuperation de la valeur finale
//   lcd_gotoxy(1,1);
//   printf(lcd_char,"MSB:%d  LSB:%d  ",dataM,dataL);	
   return(data);
}

//-------------Ecriture  I2C------------------
void ecr_i2c(byte device, byte address, byte data) {
   i2c_start();
   i2c_write(device);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
}

//-------------Lecture Boussole I2C------------------
void lecture_boussole() {
		int16 val=0;

		val=lecdb_i2c(boussole,0x41);
		val=val/10;			// récupération de la valeur avant virgule
		delay_ms(10);		// temps d'attente min. avant prochaine mesure
		lcd_gotoxy(1,2);
 		printf(lcd_char,"Degres:%ld  ",val);	
}

//-------------------------xxx------------------------
void affiche2() {
		lcd_gotoxy(1,1);
		lecture_boussole();
		delay_ms(200);
}

//-------------------------xxx------------------------
void main() {

	   	lcd_init();
	   	lcd_char("\fProjet Boussole\n");
		lcd_char("(C)FPMS - 2006");
		delay_ms(1000);	
		lcd_char("\fLecture I2c...");
		delay_ms(800);	
		lcd_cursor(0);
test:
		affiche2();
		goto test;
}