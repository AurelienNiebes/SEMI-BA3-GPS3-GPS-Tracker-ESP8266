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
void dec2bin(byte val) {							// decimal to binaire (string)
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
	   	lcd_char("\fRecepteur Capt \n");
		lcd_char("(C)UMONS - 2011");
		beep(200,30);
		delay_ms(1000);	
		lcd_cursor(0);
}
//-------------------------xxx------------------------
void lect_capteurs(){
	byte i;
	unsigned int16 val;
	signed int16 val1,val2,val3;
	float celc;

	while (bkbhit) {

		data[0]=bgetc();
		if (data[0] == entete) {					// lecture du debut de la trame (0xFF)
			data[1]=bgetc();						// lecture adresse du capteur
			data[2]=bgetc();						// lecture Type de capteur
			data[3]=bgetc();						// lecture data Lsd
			data[4]=bgetc();						// lecture data Msb
			switch (data[2]) {

				case typ_inout:											// lecture data entrees digitales
					printf(lcd_char,"\fIN Dig. Adr: %X\n",data[1]);
					dec2bin(data[3]);
					printf(lcd_char,"-> %s %2Xh",varbin,data[3]);	
					break;

				case typ_acc_x:											// lecture data accelerometre
					for (i=5;i<=14;++i) data[i]=bgetc();				// lecture autres parametres Accelerom.
					val1=data[3]+(data[4]*256);
					if ((data[5]==entete)&&(data[7]==typ_acc_y)) val2=data[8]+(data[9]*256);
					if ((data[10]==entete)&&(data[12]==typ_acc_z)) val3=data[13]+(data[14]*256);
					printf(lcd_char,"\fAcceler. Adr:%X\n",data[1]);
					printf(lcd_char,"X%ld Y%ld Z%ld  ",val1,val2,val3);	
					break;

				case typ_temp:											// lecture data temperature
					celc=data[3]+(data[4]*256);
					celc=(celc/256);
					printf(lcd_char,"\fTemper. Adr:%X\n",data[1]);
					printf(lcd_char,"-> %3.3f C ",celc);	
					break;

				case typ_light:											// lecture data lumière
					val=data[3]+(data[4]*256);
					printf(lcd_char,"\fLight Adr:%X\n",data[1]);
					printf(lcd_char,"-> %lu Lux ",val);	
					break;

				case typ_srf02:											// lecture data distance
					val=data[3]+(data[4]*256);
					printf(lcd_char,"\fDist. Adr:%X\n",data[1]);
					printf(lcd_char,"-> %lu Cm ",val);	
					break;

				case typ_adc01:											// lecture data entree ADC
					val=data[3]+(data[4]*256);
					printf(lcd_char,"\fIN ADC1 Adr:%X\n",data[1]);
					printf(lcd_char,"-> %1.1w V ",val);	
					break;

				case typ_boussole:										// lecture data boussole
					val=data[3]+(data[4]*256);
					printf(lcd_char,"\fBoussole Adr:%X\n",data[1]);
					printf(lcd_char,"-> %ld Deg. ",val);;	
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
				lcd_char("\fBluetooth ON.");
				flagp=false;
				delay_ms(100);
				while (input(bluethoot)) 					// tant que Bluetooth actif
					lect_capteurs();
			}
			
			else if (!flagp) { 
				lcd_char("\fBluetooth OFF.");
				flagp=true;
			}
		}
}
