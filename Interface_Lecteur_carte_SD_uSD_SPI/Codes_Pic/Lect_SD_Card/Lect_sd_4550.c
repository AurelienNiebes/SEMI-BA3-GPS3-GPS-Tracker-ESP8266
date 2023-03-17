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

#include "4550_sv3_scan.h"
#use spi(DI=PIN_D2, DO=PIN_D1, CLK=PIN_D0, ENABLE=PIN_D3)

#include "HDD Driver.c"

//#define _CS PIN_D3
int error,error0;
int16 rec_no;
int16 index,rec_size;
char fname[32],buff0[MMC_BUFF_SIZE+1],buff1[MMC_BUFF_SIZE+1];


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
	SETUP_SPI (SPI_MASTER |  SPI_SS_DISABLED |SPI_H_TO_L| SPI_CLK_DIV_16 | SPI_XMIT_L_TO_H);

//	set_tris_c(0b10010011); //c7=rx I, c6=tx O, c5 SDO O,c4 SDI I
	output_high(_CS);
	
	lcd_init();
   	lcd_putc("\fLecture Carte SD\n");
	lcd_putc("(C)UMONS - 2011");
	cursor(0);										// curseur OFF
	beep(200,20);
	delay_ms(1000);	
}

void test_carte()
{

	printf("\r\n**** SD / MMC FAT16  Read Demo **** ");
	printf("\r\n");
	Delay_ms(1000);
	printf("\r\Now Open HOME.TXT File on SD Card  ");
	printf("\r\n");
	Delay_ms(3000);

	buff0[MMC_BUFF_SIZE]=0;
	buff1[MMC_BUFF_SIZE]=0;
	rec_no=0;

	///////// init MMC ////////////////////////////////////////
	error=init_MMC(10);
	if (error>0) {
		goto mmc_exit;
	}

	printf("\n\r MMC initialized \n\r");
	rec_size=MMC_BUFF_SIZE;

	//strcpy(fname,"HOME\\HOME.TXT");
	strcpy(fname,"HOME.TXT");
	rec_size=MMC_BUFF_SIZE;
	error0=open_file(0,fname,rec_size);

	if (error0>0) {
		printf("\n\r fopen as 0 failed error=%U\n\r",error);
		goto mmc_exit;
	}
	else printf("\n\r opened as 0 file %s with rec size %lu \n\r",fname,rec_size);

	do {
		error0=file_read(0,buff0);
		if (error0>0 && error0<255 ) {
			printf("\n\r fread 0 failed error=%U\n\r",error0);
			break;
		}
		printf("%s",buff0);
		rec_no++;
	} while (error0==0);

	mmc_exit:
	printf("\n\r done winhex adj= %lu \n\r",winhex_adj);

}

//-------------------------xxx------------------------
void main() {

		initialisation();
		test_carte();
		while(true) {
		
		}
}
