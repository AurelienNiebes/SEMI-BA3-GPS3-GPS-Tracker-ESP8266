//************************************************************************************
//**  
//**  
//**  
//** 
//************************************************************************************

#include <18F4550.h>

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES WDT32768                	//Watch Dog Timer uses 1:128 Postscale
#FUSES HS                   	//High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES BORV20                	//Brownout reset at 2.0V
#FUSES PUT                 		//No Power Up Timer
#FUSES NOCPD                 	//No EE protection
#FUSES STVREN                	//Stack full/underflow will cause reset
#FUSES NODEBUG               	//No Debug mode for ICD
#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                 	//Program memory not write protected
#FUSES NOWRTD                	//Data EEPROM not write protected
#FUSES IESO                  	//Internal External Switch Over mode enabled
#FUSES FCMEN                 	//Fail-safe clock monitor enabled
#FUSES PBADEN                	//PORTB pins are configured as analog input channels on RESET
#FUSES NOWRTC                	//configuration not registers write protected
#FUSES NOWRTB                	//Boot block not write protected
#FUSES NOEBTR                	//Memory not protected from table reads
#FUSES NOEBTRB               	//Boot block not protected from table reads
#FUSES NOCPB                 	//No Boot Block code protection
#FUSES MCLR                  	//Master Clear pin enabled
#FUSES LPT1OSC               	//Timer1 configured for low-power operation
#FUSES NOXINST             		//Extended set extension and Indexed Addressing mode disabled (Legacy mode)
//#FUSES PLL6                  	//Divide By 6(24MHz oscillator input)
#FUSES PLL1
//#FUSES CPUDIV4               	//System Clock by 4
#FUSES CPUDIV1
//#FUSES NOUSBDIV                	//USB clock source comes from PLL divide by 2
#FUSES NOUSBDIV
#FUSES NOVREGEN                	//USB voltage regulator enabled
#FUSES NOICPRT                 	//ICPRT enabled

#use delay(clock=20M)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, stream=RS232)

#include "LCD420_S3.c"

//Defines:

	#define MX_CARD_CS 				PIN_D3
	#define MX_CARD_SCK 			PIN_D0
	#define MX_CARD_SDI 			PIN_D2		// D2
	#define MX_CARD_SDO 			PIN_D1		// D1
	
#include "fat16.c"

	#define Button PIN_E2
	#define led PIN_A4
	#define LEDOn			output_High(LED)
	#define LEDOff			output_low(LED)

	char FCV_RET_OK,code;
	char file_name[12];
	char tmp_str[16];

void main()
{
	//Initialisation
	lcd_init();
	lcd_putc("\fTest Fat\n");
	printf("\fTest Fat\n");
	delay_ms(1000);

	//Appel de la Routine Composant: ret_ok=FAT16(0)::Init_FAT
	FCV_RET_OK = FCD_FAT160_Init_FAT();

	if (FCV_RET_OK > 0)
	{
		LEDOn;
		lcd_gotoxy(1,2);
		lcd_putc("Init Fat Nogo");

	} else {
		LEDOff;
		lcd_gotoxy(1,2);
		lcd_putc("Init Fat OK");
		delay_ms(1000);
		file_name = "test.txt";
		FCV_RET_OK = FCD_FAT160_Create_File(file_name,8);
		if (FCV_RET_OK > 0)
		{
			lcd_gotoxy(1,2);
			lcd_putc("file exist    ");
		} else {
			lcd_gotoxy(1,2);
			lcd_putc("Create file OK  ");
			delay_ms(1000);
			FCV_RET_OK = FCD_FAT160_Open_File(file_name,8);
			if (FCV_RET_OK > 0)
			{
				lcd_gotoxy(1,2);
				lcd_putc("file not exist ");
			} else {
				lcd_gotoxy(1,2);
				lcd_putc("file Open OK  ");
				delay_ms(1000);
				tmp_str = "Bonjour Pic.";
				FCV_RET_OK = FCD_FAT160_Append_String_To_File(tmp_str,11);
				if (FCV_RET_OK > 0)
				{
				} else {
					sprintf(tmp_str,"\r\nTexte 2 ligne");
					FCV_RET_OK = FCD_FAT160_Append_String_To_File(tmp_str,15);
					lcd_gotoxy(1,2);
					lcd_putc("wrt to file OK");
				}
			}			
		}
	}
	lcd_gotoxy(15,1);
	lcd_putc("*");
	while(true) {
	}
}




