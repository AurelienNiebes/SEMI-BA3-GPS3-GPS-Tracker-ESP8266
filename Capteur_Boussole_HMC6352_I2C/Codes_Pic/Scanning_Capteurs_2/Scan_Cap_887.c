//-----------------------------------------------------
//------------ Programme Sendp data capt --------------
//-----------------------------------------------------
//-- Projet:                                         --
//-- Date:   10-10-2011                              --
//-- Progr:  Send data in serie 				     --
//-- Auteur: Binon Daniel (UMONS)					 --
//-----------------------------------------------------
//--   												 --
//--   												 --
//--   												 --
//--   												 --
//--   												 --
//--   												 --
//--   												 --
//--   												 --
//----------------------------------------------------- 

#include "887_sv3_scan.h"

#define temps 1000

Byte CONST temp = 0b10010000;  // Adresse  Température [TMP100] (0x90)
Byte CONST ACC = 0b00111010;  // Adresse Accéléromètre [LIS3LV02DL] (0x3A) 
Byte CONST light = 0b01000110; // Address Light [bh1710fvc-e] (0x46) 
Byte CONST SRF02 = 0xE0; // Adresse USFR [SRF02] (0xE0) 
Byte CONST boussole = 0b01000010; // Adresse Boussole [HMC6352] (0x42) 

//-------------------------xxx------------------------

signed long byte axe_x, axe_y, axe_z = 0;
char varbin[9];
byte x;
boolean flagp;

//-------------------------fonction buzzer------------------------
//permet d'emettre un son dont on peut paramétrer la fréquence et la durée

void beep(int freq, int duree)
{ 
   setup_ccp1(CCP_PWM);                  
   set_pwm1_duty(freq/2);
   setup_timer_2(T2_DIV_BY_16,freq,5);   
   while (duree--) delay_ms(10);
   setup_ccp1(CCP_OFF);
}

void initialisation() 
{
	setup_adc_ports(sAN0|sAN1|sAN2|sAN3);
	setup_adc(ADC_CLOCK_INTERNAL); 					// Config ADC
	lcd_init();
   	lcd_putc("\fScan Capteurs \n");
	lcd_putc("(C)UMONS - 2011");
	cursor(0);										// curseur OFF
	beep(200,20);
	delay_ms(1000);	
}
//-------------xxx------------------
void dec2bin(byte val) {
	byte i,x;

	x=0;
	for (i=7;i>0;i--) 
	{
		if ((val>>i)%2==0) varbin[x]='0';	
		else varbin[x]='1';
		x++;
	}	
	if ((val)%2==0) varbin[x]='0';
	else varbin[x]='1';
	varbin[x+1]='\0';
}

//-------------Lecture  I2C------------------
signed byte lec_i2c(byte device, byte address, byte del) {
   signed BYTE data;

   i2c_start();
   i2c_write(device);
   i2c_write(address);
   if (del!=0) delay_ms(del);
   i2c_start();
   i2c_write(device | 1);
   data=i2c_read(0);
   i2c_stop();
   return(data);
}

//-------------Lecture  I2C  2 bytes -----------------
signed int16 lecdb_i2c(byte device, byte address, byte del) {
   BYTE dataM,dataL;
   int16 data;

   i2c_start();
   i2c_write(device);
   i2c_write(address);
   if (del!=0) delay_ms(del);
   i2c_start();
   i2c_write(device | 1);
   dataM = i2c_read(1);				// Read MSB
   dataL = i2c_read(0);				// Read LSB
   i2c_stop();
   data=((dataM*256)+dataL);		// True value
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

//-------------Lecture Accéléromètre I2C------------------
void lecture_ACC() {
		byte tmp=0,data1,data2;
		signed int16 datax, datay, dataz=0;

		ecr_i2c(ACC,0x20,0b01000111);
		tmp=lec_i2c(ACC,0x27,0);

		if ((tmp&0x0C)==0x0C) axe_z=lec_i2c(ACC,0x2d,0);
		if ((tmp&0x0A)==0x0A) axe_y=lec_i2c(ACC,0x2b,0);
		if ((tmp&0x09)==0x09) axe_x=lec_i2c(ACC,0x29,0);

		datax = axe_x*18;
		datay = axe_y*18;
		dataz = axe_z*18;

		printf(lcd_putc,"\fCap. Accelerom. \n");
		printf(lcd_putc,"X%ld Y%ld Z%ld  ",datax,datay,dataz);
		data1=datax;data2=datax>>8;
		putc(entete);putc(adr_acc);putc(typ_acc_x);putc(data1);putc(data2);
		delay_ms(temps);
		data1=datay;data2=datay>>8;
		putc(entete);putc(adr_acc);putc(typ_acc_y);putc(data1);putc(data2);
		delay_ms(temps);
		data1=dataz;data2=dataz>>8;
		putc(entete);putc(adr_acc);putc(typ_acc_z);putc(data1);putc(data2);
}

//------------- Lecture Temperature ------------------
void lecture_temp() {
		float celcius;
		float val;
		int16 valt;
		byte data1,data2;

		ecr_i2c(temp,0x01,0x60);
		celcius=lecdb_i2c(temp,0b00000000,350);
		val=(celcius/256);
		valt=celcius;
		data1=valt;
		data2=valt>>8;
		printf(lcd_putc,"\fCap. Temperature\n");
		printf(lcd_putc,"Temp.: %3.3f C      ",val);
		putc(entete);putc(adr_temp);putc(typ_temp);putc(data1);putc(data2);
}

//-------------Lecture Light I2C------------------
void lecture_light() {
		int16 val=0;
		byte data1,data2;

		val=lecdb_i2c(light,0x20,120);    		//One Time H-Resolution Mode
		val=val/1.2;
		data1=val;								// LSB
		data2=val>>8;
		printf(lcd_putc,"\fCap. Luminosite \n");
 		printf(lcd_putc,"Lux:%ld        ",val);	
		putc(entete);putc(adr_light);putc(typ_light);putc(data1);putc(data2);
}

//-------------Lecture Distance I2C------------------
void lecture_SRF02() {
		byte t0, t1=0;
		int16 val=0;

		ecr_i2c(SRF02,0x00,0x51); // Commande
		delay_ms(80);
		t1=lec_i2c(SRF02,0x02,0);
		t0=lec_i2c(SRF02,0x03,0);
		val=t0+(t1*256);
		printf(lcd_putc,"\fCapt. Distance \n");
 		printf(lcd_putc,"Dist:%ld Cm      ",val);
		putc(entete);putc(adr_srf02);putc(typ_srf02);putc(t0);putc(t1);
}

//-------------Lecture Boussole I2C------------------
void lecture_boussole() {
		int16 val=0;
		byte data1,data2;

		val=lecdb_i2c(boussole,0x41,10);
		val=val/10;							// récupération de la valeur avant virgule
		data1=val;								// LSB
		data2=val>>8;
		printf(lcd_putc,"\fCap. Boussole   \n");
 		printf(lcd_putc,"Degres:%ld       ",val);
		putc(entete);putc(adr_boussole);putc(typ_boussole);putc(data1);;putc(data2);
}

//------------------------xxx---------------------
void lecture_ADC() {
	byte i,data1,data2;
	long int valeur,val[5];

			
		for (i=0;i<4;++i) {
			set_adc_channel(i);							// selection canal 1
			delay_us(10);
			valeur= read_ADC();
			val[i]=((valeur*4.88)/100);
		}		
		data1=val[1];
		data2=val[1]>>8;
		printf(lcd_putc,"\fEntrees ADC     \n");
		printf(lcd_putc,"%1.1w %1.1w %1.1w %1.1w ",val[0],val[1],val[2],val[3]);
		putc(entete);putc(adr_inout);putc(typ_adc01);putc(data1);putc(data2);
}

//------------------------xxx---------------------
void lecture_port() {
	byte data;

		data=input_d();
		printf(lcd_putc,"\fEntrees Digital \n");
		dec2bin(data);
		printf(lcd_putc,"%sb %2Xh",varbin,data);
		putc(entete);putc(adr_inout);putc(typ_inout);putc(data);putc(0);
}

//-------------------------xxx------------------------
void lect_capteurs(byte cap) {

		switch (cap) {

			case 0: 
				lecture_temp();
				break;

			case 1:
				lecture_light();
				break;

			case 2:
				Lecture_acc();
				break;

			case 3:
				Lecture_SRF02();
				break;

			case 4:
				lecture_boussole();
				break;

			case 5:
				lecture_adc();
				break;

			case 6:
				lecture_port();
				break;


			default :
				break;
		}
}

//-------------------------xxx------------------------
void main() {

		initialisation();
		x=0;
		flagp=false;
		while(true) {
			if (input(bluethoot)) {							// RF bluetooth actif ?
				lcd_putc("\fBluetooth ON.");
				flagp=false;
				delay_ms(100);
				while (input(bluethoot))					// tant que Bluetooth actif
				{ 					
					x=(x+1) % 7;
					lect_capteurs(x);
					delay_ms(temps);
				}
			}
			else if (!flagp) { 
				lcd_putc("\fBluetooth OFF.");
				flagp=true;
			}
		}
}
