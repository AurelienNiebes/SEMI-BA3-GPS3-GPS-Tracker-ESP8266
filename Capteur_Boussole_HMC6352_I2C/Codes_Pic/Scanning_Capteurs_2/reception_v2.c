// file reception.c
//-----------------------------------------------------
//------------ Programme Receip data capt -------------
//-----------------------------------------------------
//-- Projet:
//-- Date:   10-10-2011
//-- Progr:  Capture data in serie
//-- Auteur: Binon Daniel (UMONS)
//-----------------------------------------------------
//-- Réception des données envoyées par l'émmetteur. 
//-- Ces données sont codées selon un format définit 
//-- plus haut (voir fichier "887_sv3_rser.h")
//-- on récupère celles-ci et on les affichent sur LCD
//-- 
//-- 
//-- 
//-- 
//----------------------------------------------------- 

#include "887_sv3_rser.h"							// ..\include\

//-------------------------xxx------------------------

BYTE buffer[BUFFER_SIZE];
BYTE next_in = 0;
BYTE next_out = 0;
char varbin[9];
byte data[16];
boolean flagp;

//-------------------------xxx------------------------
#int_rda											// interruption serie
void serial_isr() {
   int t;

   buffer[next_in]=getc();							// remplissage du buffer
   t=next_in;
   next_in=(next_in+1) % BUFFER_SIZE;
   if(next_in==next_out)
     next_in=t;           							// Buffer full !!
}

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

//-------------------------xxx------------------------
char* dec2bin(byte pbyte) {							// decimal to binaire (string)
	byte i,x;

	x=0;
	for (i=7;i>0;i--) 
	{
		if ((pbyte>>i)%2==0) varbin[x]='0';
		else varbin[x]='1';
		x++;
	}	
	if ((pbyte)%2==0) varbin[x]='0';
	else varbin[x]='1';
	varbin[x+1]='\0';
	return varbin;
}

//-------------------------xxx------------------------
BYTE bgetc() {										// lecture du buffer serie
   BYTE c;

   while(!bkbhit) ;									// tant que le buffer n'est pas vide
   c=buffer[next_out];								// lecture du buffer
   next_out=(next_out+1) % BUFFER_SIZE;				// mise a jour du pointeur
   return(c);
}

//-------------------------xxx------------------------
void initialisation() {

   		enable_interrupts(int_rda);					// interrupt for serial ON
		enable_interrupts(global);					// enable interrupt
		lcd_init();
		lcd_init_custom_chars();
	   	lcd_putc("\fRecepteur Capt \n");
		lcd_putc("(C)UMONS - 2011");
		cursor(0);								// curseur OFF
		beep(200,30);
		delay_ms(1000);	
}
//-------------------------xxx------------------------
void lect_capteurs(){
	unsigned int16 v16u;
	signed   int16 v16s;
	float          vfloat;
	char* binstring;

	while (bkbhit) {

		data[0]=bgetc();
		if (data[0] == DATA_HEAD) {					// lecture du debut de la trame (0xFF)
			data[1]=bgetc();	// lecture adresse du capteur
			data[2]=bgetc();	// lecture Type de capteur
			data[3]=bgetc(); 	// lecture data byte 1
			data[4]=bgetc(); 	// lecture data byte 2
			switch (data[2]) {

				case DATA_TYPE_INOUT: 	// lecture data entrees digitales
					printf(lcd_putc,"\fINport ADDR(%X)\n",data[1]);
					binstring = dec2bin(data[3]);
					printf(lcd_putc,"b%s(h%2X)",binstring,data[3]);	
					break;

				case DATA_TYPE_ACCX: 	// lecture data accelerometre
					printf(lcd_putc,"\fACCx ADDR(%X)\n",data[1]);
					v16s=data[3]+(data[4]*256);
					printf(lcd_putc,"%ld[mG]",v16s);	
					break;

				case DATA_TYPE_ACCY: 	// lecture data accelerometre
					printf(lcd_putc,"\fACCy ADDR(%X)\n",data[1]);
					v16s=data[3]+(data[4]*256);
					printf(lcd_putc,"%ld[mG]",v16s);	
					break;

				case DATA_TYPE_ACCZ: 	// lecture data accelerometre
					printf(lcd_putc,"\fACCz ADDR(%X)\n",data[1]);
					v16s=data[3]+(data[4]*256);
					printf(lcd_putc,"%ld[mG]",v16s);	
					break;
					
				case DATA_TYPE_TEMPER:											// lecture data temperature
					vfloat= ((float)(data[3]+(data[4]*256)))/256; 
					printf(lcd_putc,"\fTEMPER ADDR(%X)\n",data[1]);
					printf(lcd_putc,"%3.3f[C\1]",vfloat);	
					break;

				case DATA_TYPE_LIGHT:											// lecture data lumière
					v16u=data[3]+(data[4]*256);
					printf(lcd_putc,"\fLIGHT ADDR(%X)\n",data[1]);
					printf(lcd_putc,"%lu[Lux]",v16u);	
					break;

				case DATA_TYPE_SRF2:											// lecture data distance
					v16u=data[3]+(data[4]*256);
					printf(lcd_putc,"\fDIST. ADDR(%X)\n",data[1]);
					printf(lcd_putc,"%lu[cm]",v16u);	
					break;

				case DATA_TYPE_ADC1:											// lecture data entree ADC
					v16u=data[3]+(data[4]*256);
					printf(lcd_putc,"\fADC1 ADDR(%X)\n",data[1]);
					printf(lcd_putc,"%1.1w[V]",v16u);	
					break;

				case DATA_TYPE_COMPASS:										// lecture data boussole
					v16u=data[3]+(data[4]*256);
					printf(lcd_putc,"\fCOMPASS ADDR(%X)\n",data[1]);
					printf(lcd_putc,"%ld[\1]",v16u);	
					break;

				default :
					while (bkbhit) {
						bgetc();										// vidage buffer si non OK
					}
					break;
			}	
		}  
	}
}

//-------------------------xxx------------------------
void main() {

		initialisation();
		flagp=false;
		while(true) {
			if (input(bluethoot)) {							// RF bluetooth actif ?
				lcd_putc("\fBluetooth ON.");
				flagp=false;
				delay_ms(100);
				while (input(bluethoot)) 					// tant que Bluetooth actif
					lect_capteurs();
			}
			
			else if (!flagp) { 
				lcd_putc("\fBluetooth OFF.");
				flagp=true;
			}
		}
}
