// This example open a file for append, and writes "System
// started". It also shows small error handling            
/*
	char f,gfilename[12];  

	strcpy(gfilename,"EVENTS.LOG");
	f = fopen(gfilename,'a'); 		// open EVENTS.LOG for append
	if (f & MMC_ERROR)
	{
		printf("Couldn't open file!\r\n");
		if(f == MMC_NO_CARD_INSERTED)
			printf("Please, insert MMC!");
		else if(f == MMC_MAX_FILES_REACHED)
			printf("ops.. =)");
	}
	else
	{
		strcpy(msg,"System started\r\n");
		fputstring(msg,f);
		fclose(f);
	}
*/
	// Here is a exampel that covers almost everything...      
	// It first lists all files and dirs in DIR/ and then open 
	// The first and prints the file with printf, take a look: 
#include <18F4550.h>
#fuses HS,NOWDT,NOPROTECT,NOPUT,NOLVP,NOBROWNOUT,NOWRT

#use delay(clock=24 000 000)
#use rs232(baud=9600, bits=8, parity = N, xmit=PIN_C6, rcv=PIN_C7)
#include <string.h>

#include <MMC_SPI_FAT32.h> 	// As usual you also need to 
#include <MMC_SPI_FAT32.c> 	// include your device .h file... 

void main(void)
{
	char f,filename[20],res,i,c;
	
	while(TRUE)
	{
    if(MMCInit() == MMC_OK)
    {
		printf("MMC initialized\r\n");
		InitFAT();
		strcpy(filename,"DIR/");
		f = InitList(filename);
		if(f & MMC_ERROR)
			printf("Error");
		else{
			do{
				res = ListFiles(f);
				for(i=0;i<res;i++){
					printf(FileList[i].name);
					if(FileList[i].isDir)
						printf("\\\r\n");   //a "\" with row break
					else
						printf("\r\n");
					}
			} while(NextPage(f) == MMC_OK);
			CloseList(f); // frees the file, but the list is
		}                 // still there...
		strcpy(filename,FileList[0].shortName);
		f = fopen(filename,'r');  // open file for reading
		if((f & MMC_ERROR) == 0) // No error, same as
		{                        // if(f < MAXFILES)
			while(fgetch(&c,f) == MMC_OK)
				printf(c);
			fclose(f);
		}
		else if(f == MMC_NOT_FOUND)
				printf("1st file in list was probebly a dir");
			else
				printf("Other error\r\n");
			while(TRUE)
              ; 	// Loop forever, program is finished
			}
			else
				printf("MMC init failed!\r\n");
				delay_ms(1000);
				printf("Trying once more..\r\n");
		}
}                               