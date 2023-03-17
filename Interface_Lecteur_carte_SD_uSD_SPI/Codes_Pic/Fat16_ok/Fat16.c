// ----------------------------------------------------------------------
// -----      Driver gestion Fat16 tiré du code C de BootC          -----
// -----         (C) UMONS SERVICE Electronique(SEMI)               -----
// -----  Nom: Daniel Binon                                         -----
// -----  Date: 08-02-2013                                          -----
// ----------------------------------------------------------------------
// FAT160_Init_FAT();	Initializes the media.
//
// FAT160_Open_File(F, L);
//  Searches for a file in the current directory and reads the first sector 
//  of the file into memory.
//  	* Prerequisites - Init_FAT
// 		* Parameters - F = Filename string, L = Long string filename
//  	* Retval = 0 - File found and opened - 512-byte buffer contains 1st 
//					   sector of file
//  	* Retval = 1 - File not found
//  	* Retval > 1 - Error
//
// FAT160_Read_File_Sector();
//  Overwrites local buffer with the current file sector from the disk
//  Used to restore the local sector buffer with the actual sector stored 
//  on the card.
//		* Prerequisites - Init_FAT, Open_File
//		* Optional - Move_To_Next_Sector
//		* Retval = 0 - Local buffer refreshed with current card sector 
//		* Retval > 0 - Error
//
// FAT160_Read_File_Length();
//  Returns the number of bytes in the current sector that are used 
//  by the current open file. 
//		* Prerequisites - Init_FAT, Open_File
//		* Retval = 512 - Sector is filled with file data
//		* Retval < 512 - Last sector of the file, Retval specifies 
//						 how many bytes of data are remaining in the sector
//
// FAT160_Move_To_Next_File_Sector(Force_Next_Sector);
//  Overwrites local buffer with the next file sector from the disk
//  Used to read the next chunk of a file.
//  If the force next sector parameter is set to 0 then when the last sector
//  is reached the function will remain in the last valid sector of the file.
//  If the force next sector parameter is set to 1 then when the last sector
//  is reached the function will create a new sector at the end of the file 
//  and update the size parameter of the file.
//		* Prerequisites - Init_FAT, Open_File
//		* Parameters - Force_Next_Sector
//		* Retval = 0 - Sector read or appended correctly
//		* Retval > 0 - Error
//
// FAT160_Append_String_To_File("String",L);
//  Appends a string or byte array of data to the end of the file that is 
//  currently loaded and updates the size parameter of the file.
//		* Prerequisites - Init_FAT, Open_File
//		* Parameters - String - Contains the String or byte array to be appended 
//						to the end of the file and L = Long string
//		* Retval =  0 - String or byte array has been appended correctly
//		* Retval > 0 - An error occured while appending the string.
//
// FAT160_Write_File_Sector();
//	Overwrites the current sector on the memory card with the contents of the local
//  sector buffer. See write_byte_to_buffer function.
//		* Prerequisites - Init_FAT, Open_File, Write_Byte_To_Buffer
//		* Optional - Move_To_Next_File_Sector
//		* Parameters - String - Contains the String or byte array to be appended to 
//						the end of the file 
//		* Retval = 0 - File sector written successfully.
//		* Retval > 0 - File sector write error
//
// FAT160_Create_File("string", L);
//  Creates a file with the specified filename in the current directory of the card.
//  Also assigns the file certain parameters such as create time and date and size etc.
//		* Prerequisites - Init_FAT
//		* Optional - Open_Folder
//		* Parameters - String - Contains the String or byte array to be appended to the 
//						end of the file L = Long string
//		* Retval = 0 - File created successfully
//		* Retval > 0 - Directory table of the current folder is full.
//
// FAT160_Delete_File("string", L);
//  Scans the current directory on the card for a specific filename. If the filename is 
//  found then the file is removed from the disk by destroying the FAT entries in the 
//  directory entry.
//		* Prerequisites - Init_FAT
//		* Optional - Open_Folder
//		* Parameters - String - Contains the name of the file to be deleted 
//						and L = Lon string
//		* Retval = 0 - File found and deleted.
//		* Retval > 0 - File not found
//
// FAT160_Write_Byte_To_Buffer(Address, Data);
//  Modifies the value of a byte in the local sector buffer.
//		* Prerequisites - Init_FAT, Open_File
//		* Optional - Move_To_Next_File_Sector, Read_File_Sector
//		* Parameters - Address = 0 - 511 - Contains the position of the byte to be updated. 
//		* Parameters - Data - Contains the byte value that will be written to the local sector
//
// FAT160_Read_Byte_From_Buffer(Address);
//  Reads a byte of data from the local sector buffer.
//		* Prerequisites - Init_FAT, Open_File
//		* Optional - Move_To_Next_File_Sector, Read_File_Sector
//		* Parameters - Address = 0 - 511 - Contains the position of the byte to be read.
//		* Retval = Data byte read from the local sector buffer. 
//
// FAT160_Open_Folder("string", L);
//  Scans the current directory on the card for a specific folder name. If the folder name
//  is found then the card opens the folder and makes it the current directory.
//		* Prerequisites - Init_FAT
//		* Parameters - String - Contains the name of the folder to be opened.
//		* String = "root" - Go directly to Root folder and L = long string
//		* String = ".." - Go up a directory
//		* Retval = 0 - Folder found and opened
//		* Retval > 0 - Folder not found 
//
// FAT160_Scan_Current_Folder(Current_Idx, Scan_Type);
//  Method for retreiving file or folder names for the card to allow for dynamic file 
//  opening or playback.
//		* Prerequisites - Init_FAT
//		* Optional - Open_Folder
//		* Parameters - Current_Idx - Contains the current pointer in the directory table.
//		* Parameters - Scan_Type - Sets the scan to search for Files or Folders.
//  		Current_Idx - Call the function with values from 0 to 511 to completley search the 
//			current folder for a files or folders
//			Scan_Type - 0 = Scan for files, 1 = Scan for folders
//		* Retval = 0 - File or folder found and result stored into string array.
//		* Retval = 1 - End of folder reached. 
//		* Retval = 2 - Filetype in the directory table does not match the Scan_Type parameter. 
//  	* Retval = 3 - Current section of the Filetable is used for an extended filename or is empty. 
//
// FAT160_Read_Byte_From_Scan(idx);
//  Returns a byte of the name of the file or folder retreived from the Scan_Current_folder function.
//		* Prerequisites - Init_FAT, Scan_Current_Folder
//		* Parameters - Idx - Contains the current pointer in the file or folder name retreived from the last scan
//			Idx - 0 - 11 for a file returns the 8.3 character file name.
//			Idx - 0 - 7 for a folder returns the 8 character folder name#
//		* Retval = ASCII byte at position Idx from the folder name or file name at position 
//			Current_Idx in the directory
//
// FAT160_MX_SPI_BYTE(shift);
//
// FAT160_MX_Init_Card();
//
// FAT160_MX_Send_Buffer(address);
//
// FAT160_MX_Get_Buffer(address);
//
// FAT160_MX_Format_File_String("string", L);
//
// FAT160_MX_Add_Sector_To_File();
//
// FAT160_MX_Clear_Buffer();
//
// FAT160_Read_Next_File_Sector();
// ----------------------------------------------------------------------


/**** Macro Substitutions ****
16953 = File Creation Time
32038 = File Creation Date
0 = FAT System Type - FAT16 Only
******************************/

#ifndef MX_CARD_SCK
	#define MX_CARD_CS 				PIN_D3		// Output
	#define MX_CARD_SCK 			PIN_D0		// Output
	#define MX_CARD_SDI 			PIN_D2		// Input
	#define MX_CARD_SDO 			PIN_D1		// Output
#endif

	#define MX_FILE_CREATEDATE		16953
	#define MX_FILE_CREATETIME		32038
	#define MX_FAT_SYST				0
	#define MX_EXT_NAME				0

	//Common Defines
	#define MX_ERR_NOCARD			0xFF
	#define MX_ERR_NORESP			0xFE
	#define MX_ERR_NOACK			0xFD
	#define MX_ERR_TOUT				0xFC
	#define MX_ERR_RESERVED			0xFB
	#define MX_ERR_BAD				0xFA
	#define MX_ERR_FREE				0xF0
	#define MX_ERR_FILENOTFOUND		0xEF
	#define MX_ERR_ROOTFULL			0xEE
	#define MX_ACK_FILEEMPTY		0x03
	#define MX_ACK_FILETYPE			0x02
	#define MX_ACK_EOF				0x01
	#define MX_ACK_OK				0x00

#TYPE   SHORT=16, INT=16, LONG=32			//be careful,type change

////////////////////////
///                  ///
/// Global Variables ///
///                  ///
////////////////////////

	char mx_card_bufferl[256];				//512-Byte Global Buffer Variables
	char mx_card_bufferh[256];
	char mx_name_ext[12];					//11 byte fat file name: 8 char filename + 3 char extension
	char mx_file_entry;						//Directory entry of file
	unsigned int mx_root_sectors = 0;		//Number of sectors in the root directory
	unsigned int mx_cluster = 2;			//Current data mx_cluster in the memory
	unsigned int mx_fat_size = 0;			//FAT size in sectors
	unsigned long mx_fat_start = 0;			//Start address of FAT tables
	unsigned long mx_root_start = 0;		//Start address of root directory
	unsigned long mx_data_start = 0;		//Start address of the data
	unsigned long mx_file_size = 0;			//Size of file in bytes
	unsigned long mx_file_address = 0; 		//Start address of File
	unsigned long mx_file_sector = 1;		//Current sector of file
	unsigned long mx_folder_size = 0;		//Folder size in sectors
	unsigned long mx_folder_start = 0;		//Start address of Folder
	unsigned long mx_current_address = 0;	//Current address loaded in memory


//FAT16(0): //Déclarations de fonction Macro

char FCD_FAT160_Init_FAT();
char FCD_FAT160_Open_File(char* File_Name, char MSZ_File_Name);
char FCD_FAT160_Read_File_Sector();
short FCD_FAT160_Read_File_Length();
char FCD_FAT160_Move_To_Next_File_Sector(char Force_Next_Sector);
char FCD_FAT160_Append_String_To_File(char* String, char MSZ_String);
char FCD_FAT160_Write_File_Sector();
char FCD_FAT160_Create_File(char* File_Name, char MSZ_File_Name);
char FCD_FAT160_Delete_File(char* File_Name, char MSZ_File_Name);
void FCD_FAT160_Write_Byte_To_Buffer(short Address, char Data);
char FCD_FAT160_Read_Byte_From_Buffer(short Address);
char FCD_FAT160_Open_Folder(char* Folder_Name, char MSZ_Folder_Name);
short FCD_FAT160_Scan_Current_Folder(short Current_Idx, char Scan_Type);
char FCD_FAT160_Read_Byte_From_Scan(char idx);
char FCD_FAT160_MX_SPI_BYTE(char shift);
char FCD_FAT160_MX_Init_Card();
char FCD_FAT160_MX_Send_Buffer(long address);
char FCD_FAT160_MX_Get_Buffer(long address);
void FCD_FAT160_MX_Format_File_String(char* File_Name, char MSZ_File_Name);
void FCD_FAT160_MX_Add_Sector_To_File();
void FCD_FAT160_MX_Clear_Buffer();
char FCD_FAT160_Read_Next_File_Sector();

void FCI_TOUPPER(char* sSrc, char iSrc_len, char* sDst, char iDst_len){
		char idx;
		char ch;

		//copy source into destination and change to upper case
		for (idx=0; idx<iSrc_len; idx++){
			if (idx < iDst_len){
				ch = sSrc[idx];
				sDst[idx] = ch;
				if (ch != 0){
					if ((ch >= 'a') && (ch <= 'z')){
						sDst[idx] = (ch & 0xDF);
					}
				} else {
					break;
				}
			}
		}
}

char FCI_COMPARE(char* sSrc1, char iSrc1_len, char* sSrc2, char iSrc2_len, char iNoCase){
		char idx;
		char ch1, ch2;

		for (idx=0; idx < iSrc1_len; idx++){	//compare a byte from the strings
			if (idx < iSrc2_len){
				ch1 = *sSrc1;
				ch2 = *sSrc2;
				if (iNoCase){
					if ((ch1 >= 'a') && (ch1 <= 'z'))
						ch1 = (ch1 & 0xDF);

					if ((ch2 >= 'a') && (ch2 <= 'z'))
						ch2 = (ch2 & 0xDF);
				}
				if (ch1 < ch2){
					return(255);
				}
				else if (ch1 > ch2){
					return(1);
				}
				sSrc1++;
				sSrc2++;
			}
			else{
				if (*sSrc1 == 0)
					return (0);				//end of source1 as well, so ok
				else
					return (1);				//reached the end of iSrc2, but still more of iSrc1
			}
		}
		if (iSrc1_len == iSrc2_len)			//reached the end of iSrc1.  If we're also at the end of iSrc2, then return 0
			return (0);
		else{
			if (*sSrc2 == 0)
				return (0);					//end of source1 as well, so ok
			else
				return (255);				//not at end of source2, so return -1
		}
}

//FAT16(0): //Implémentations Macro

char FCD_FAT160_Init_FAT(){
		char retval;
		char num_fats, sects_per_clust;
		unsigned int bytes_per_sect = 0;
		unsigned int reserved_sectors;
		unsigned long partition_start = 0;  								//Start address of the partition
		unsigned long root_size;											//Number of 32 byte root entries.

		retval = FCD_FAT160_MX_Init_Card();									//Startup Card
		if (retval)
			return(retval);													//Startup error or no card
		retval = FCD_FAT160_MX_Get_Buffer(0);								//Read Master Boot Record if available
		if (retval)
			return(retval);
		while (bytes_per_sect == 0)											//Search for valid partition table
		{
			partition_start = FCD_FAT160_Read_Byte_From_Buffer(0x01C9);						//Read start address of partition
			partition_start = (partition_start << 8) | FCD_FAT160_Read_Byte_From_Buffer(0x01C8);
			partition_start = (partition_start << 8) | FCD_FAT160_Read_Byte_From_Buffer(0x01C7);
			partition_start = (partition_start << 8) | FCD_FAT160_Read_Byte_From_Buffer(0x01C6);

			retval = FCD_FAT160_MX_Get_Buffer(partition_start);				//Read Start of Partition (should also work for disks with no MBR or extended partitions)
			
			bytes_per_sect = FCD_FAT160_Read_Byte_From_Buffer(0x0B);								//Bytes per sector - normally 512
			bytes_per_sect = bytes_per_sect | ((int) FCD_FAT160_Read_Byte_From_Buffer(0x0C) << 8);
		}
		sects_per_clust = FCD_FAT160_Read_Byte_From_Buffer(0x0D);								//Read number of sectors per mx_cluster
		reserved_sectors = FCD_FAT160_Read_Byte_From_Buffer(0x0E);								//Number of reserved sectors
		reserved_sectors = reserved_sectors | ((int) FCD_FAT160_Read_Byte_From_Buffer(0x0F) << 8);
		num_fats = FCD_FAT160_Read_Byte_From_Buffer(0x10);										//Num of copies of FAT - normally 2
		root_size = FCD_FAT160_Read_Byte_From_Buffer(0x11);										//Size of root directory x 32 byte file/directory entries
		root_size = root_size | ((int) FCD_FAT160_Read_Byte_From_Buffer(0x12) << 8);
		if(root_size == 0)													//if root size not defined here
		{
			root_size = FCD_FAT160_Read_Byte_From_Buffer(0x24);									//Size of root directory x 32 byte file/directory entries
			root_size = root_size | ((int) FCD_FAT160_Read_Byte_From_Buffer(0x25) << 8);
			root_size = root_size | ((long)  FCD_FAT160_Read_Byte_From_Buffer(0x26) << 16);
			root_size = root_size | ((long)  FCD_FAT160_Read_Byte_From_Buffer(0x27) << 24);
		}
		mx_fat_size = FCD_FAT160_Read_Byte_From_Buffer(0x16);										//FAT size in sectors
		mx_fat_size = mx_fat_size | ((int) FCD_FAT160_Read_Byte_From_Buffer(0x17) << 8);

		mx_fat_start = partition_start + reserved_sectors;						//start of FAT address space in sectors
		mx_root_sectors = (root_size * 32) / 512;								//Number of sectors in the root directory
		mx_root_start = mx_fat_start + (num_fats * mx_fat_size);						//start of the root directory in sectors
		mx_data_start = mx_root_start + mx_root_sectors;								//start of the data mx_clusters in sectors

		mx_folder_start = mx_root_start;
		mx_folder_size = mx_root_sectors;
		return retval;
}

char FCD_FAT160_Open_File(char* File_Name, char MSZ_File_Name){
	
		char i;
		char name_comp[11];
		int idx_fat;
		unsigned int scount;
		unsigned int fat_pointer = 0xFFFF;
		unsigned long fat_address;

		FCD_FAT160_MX_Format_File_String(File_Name, MSZ_File_Name);						//Convert filename to 8.3 format
		scount = 0;
		mx_current_address = mx_folder_start;											//Move to start of root
		while (scount < mx_folder_size)
		{
			FCD_FAT160_MX_Get_Buffer(mx_current_address);										//Read sector of root directory
			for(i=0;i<16;i++)													//16 file or directory references per sector
			{
				name_comp[0] = FCD_FAT160_Read_Byte_From_Buffer(0 + (i*32));							//Move DOS 8.3 Filename into 2nd comparison buffer
				name_comp[1] = FCD_FAT160_Read_Byte_From_Buffer(1 + (i*32));
				name_comp[2] = FCD_FAT160_Read_Byte_From_Buffer(2 + (i*32));
				name_comp[3] = FCD_FAT160_Read_Byte_From_Buffer(3 + (i*32));
				name_comp[4] = FCD_FAT160_Read_Byte_From_Buffer(4 + (i*32));
				name_comp[5] = FCD_FAT160_Read_Byte_From_Buffer(5 + (i*32));
				name_comp[6] = FCD_FAT160_Read_Byte_From_Buffer(6 + (i*32));
				name_comp[7] = FCD_FAT160_Read_Byte_From_Buffer(7 + (i*32));
				name_comp[8] = FCD_FAT160_Read_Byte_From_Buffer(8 + (i*32));
				name_comp[9] = FCD_FAT160_Read_Byte_From_Buffer(9 + (i*32));
				name_comp[10] = FCD_FAT160_Read_Byte_From_Buffer(10 + (i*32));

				if (FCI_COMPARE(mx_name_ext,11,name_comp,11,1) == 0)				//Perform filename comparison
				{
					mx_file_entry = i;												//Store file address
					mx_file_address = mx_current_address;
					mx_file_sector = 1;

					mx_cluster = FCD_FAT160_Read_Byte_From_Buffer(26 + (i*32));							//Read Cluster of data start
					mx_cluster = mx_cluster | ((int) FCD_FAT160_Read_Byte_From_Buffer(27 + (i*32)) << 8);
					mx_file_size = FCD_FAT160_Read_Byte_From_Buffer(28 + (i*32));						//Read Filesize in bytes
					mx_file_size = mx_file_size | ((int) FCD_FAT160_Read_Byte_From_Buffer(29 + (i*32)) << 8);
					mx_file_size = mx_file_size | ( (long) FCD_FAT160_Read_Byte_From_Buffer(30 + (i*32)) << 16);
					mx_file_size = mx_file_size | ( (long) FCD_FAT160_Read_Byte_From_Buffer(31 + (i*32)) << 24);

					if (mx_cluster == 0)											//Empty File - Assign a free mx_cluster
					{
						idx_fat = 1;
						fat_address = mx_fat_start;								//Move to start of FAT
						FCD_FAT160_MX_Get_Buffer(fat_address);								//Read FAT sector
						while (fat_pointer != 0)								//Scan FAT for free entry
						{
							idx_fat = idx_fat + 1;
							if(idx_fat == 256)									//If at the end of the FAT sector
							{
								fat_address = fat_address + 1;
								FCD_FAT160_MX_Get_Buffer(fat_address);						//Read next FAT sector
								idx_fat = 0;
							}
							fat_pointer = FCD_FAT160_Read_Byte_From_Buffer(idx_fat * 2);				//Extract data mx_cluster address
							fat_pointer = fat_pointer | ((int) FCD_FAT160_Read_Byte_From_Buffer((idx_fat * 2) + 1) << 8);
						}

						FCD_FAT160_Write_Byte_To_Buffer((idx_fat * 2), 0xFF);						//Write end of file marker into new FAT address
						FCD_FAT160_Write_Byte_To_Buffer(((idx_fat * 2) + 1), 0xFF);
						FCD_FAT160_MX_Send_Buffer(fat_address);
						FCD_FAT160_MX_Send_Buffer(fat_address + mx_fat_size);					//Mirror data onto FAT 2

						FCD_FAT160_MX_Get_Buffer(mx_current_address);							//Read sector of root directory containing file
						fat_address = ((fat_address - mx_fat_start) * 256) + idx_fat;
						mx_cluster = fat_address & 0xFFFF;
						FCD_FAT160_Write_Byte_To_Buffer(26 + (i*32), mx_cluster);						//Write new mx_cluster value
						FCD_FAT160_Write_Byte_To_Buffer(27 + (i*32), (mx_cluster >> 8));
						FCD_FAT160_MX_Send_Buffer(mx_current_address);						//Write mx_cluster location to card

					}

					mx_current_address = mx_data_start + ((mx_cluster - 2) * 32);		//Move to sector containing start of file
					FCD_FAT160_MX_Get_Buffer(mx_current_address);								//Read sector
					return MX_ACK_OK;
				}
			}
			mx_current_address = mx_current_address + 1;
			scount = scount + 1;												//Move to next sector of folder
		}
		return MX_ERR_FILENOTFOUND;												//File not found
}

char FCD_FAT160_Read_File_Sector()
{
		return FCD_FAT160_MX_Get_Buffer(mx_current_address);		//Read sector
}

short FCD_FAT160_Read_File_Length()
{
		long temp;
		temp = mx_file_sector << 9;
		if (temp <= mx_file_size)					//If file size greater then sector limit
			return 512;								//Return Sector Full

		temp = mx_file_size % 512;
		return (temp);								//Return index of next available byte
}

char FCD_FAT160_Move_To_Next_File_Sector(char Force_Next_Sector)
{
		char retval;
		retval = FCD_FAT160_Read_Next_File_Sector();				//Try to move to the next sector
		if (Force_Next_Sector)							//If forcing next sector
		{
			if(retval != 0x01)
				return retval;							//Next sector exists and has been read (0) or error with sector chain

			FCD_FAT160_MX_Add_Sector_To_File();					//If last sector in FAT chain then create new link
			mx_file_sector = mx_file_sector + 1;		//Increment file sector
			return MX_ACK_OK;
		}
		return retval;
}

char FCD_FAT160_Append_String_To_File(char* String, char MSZ_String)
{
		char idx;
		short length;
		char retval;
		char offset = 0;

		for (idx=0;idx<MSZ_String;idx++)
		{
			if(String[idx] == 0)
				MSZ_String = idx;
		}

		length = FCD_FAT160_Read_File_Length();				//Get next empty character position
		while (length == 512)						//While characters in sector full
		{
			retval = FCD_FAT160_Read_Next_File_Sector();		//Load next sector
			if (retval == 0x01)						//If end of file then next sector will be empty
				length = -1;
			else
				length = FCD_FAT160_Read_File_Length();		//Else end of data located in this sector
		}

		if (length == -1)
			length = 512;

		for (idx = 0; idx < MSZ_String; idx++)		//Loop through each character in string
		{
			if((length + idx) == 512)				//if string extends onto another sector
			{
				length = 0;
				offset = idx;

				FCD_FAT160_MX_Send_Buffer(mx_current_address);			//Write string data in current sector to card
				retval = FCD_FAT160_Read_Next_File_Sector();		//Load next sector

				if (retval == 0x01)
				{
					FCD_FAT160_MX_Add_Sector_To_File();			//If last sector in FAT chain then create new link
				}

				if (retval > 0x01)
					return retval;										//Error occurred - return error
			}

			FCD_FAT160_Write_Byte_To_Buffer((length + (idx - offset)), String[idx]);		//Write character from string into buffer
		}

		FCD_FAT160_MX_Send_Buffer(mx_current_address);								//Write string data in current sector to card
		mx_file_size = mx_file_size + MSZ_String;								//Update file length
		FCD_FAT160_MX_Get_Buffer(mx_file_address);									//Read file address
		FCD_FAT160_Write_Byte_To_Buffer((28 + (mx_file_entry * 32)), mx_file_size);				//Update file length in buffer
		FCD_FAT160_Write_Byte_To_Buffer((29 + (mx_file_entry * 32)), mx_file_size >> 8);
		FCD_FAT160_Write_Byte_To_Buffer((30 + (mx_file_entry * 32)), mx_file_size >> 16);
		FCD_FAT160_Write_Byte_To_Buffer((31 + (mx_file_entry * 32)), mx_file_size >> 24);
		FCD_FAT160_MX_Send_Buffer(mx_file_address);									//Update file length on card
		FCD_FAT160_MX_Send_Buffer(mx_file_address + mx_fat_size);						//Mirror data onto FAT 2
		FCD_FAT160_MX_Get_Buffer(mx_current_address);									//Read current sector
		return MX_ACK_OK;														//String appended to file
}

char FCD_FAT160_Write_File_Sector()
{
		if ((mx_file_size >> 9) == (mx_file_sector - 1))	//Is this the last sector of the file
			mx_file_size = (mx_file_size + (512 - FCD_FAT160_Read_File_Length()));

		FCD_FAT160_MX_Send_Buffer(mx_current_address);								//Write string data in current sector to card
		FCD_FAT160_MX_Get_Buffer(mx_file_address);									//Read file address
		FCD_FAT160_Write_Byte_To_Buffer((28 + (mx_file_entry * 32)), mx_file_size);				//Update file length in buffer
		FCD_FAT160_Write_Byte_To_Buffer((29 + (mx_file_entry * 32)), mx_file_size >> 8);
		FCD_FAT160_Write_Byte_To_Buffer((30 + (mx_file_entry * 32)), mx_file_size >> 16);
		FCD_FAT160_Write_Byte_To_Buffer((31 + (mx_file_entry * 32)), mx_file_size >> 24);
		FCD_FAT160_MX_Send_Buffer(mx_file_address);									//Update file length on card
		FCD_FAT160_MX_Send_Buffer(mx_file_address + mx_fat_size);						//Mirror data onto FAT 2
		FCD_FAT160_MX_Get_Buffer(mx_current_address);									//Read current sector

		return MX_ACK_OK;
}

char FCD_FAT160_Create_File(char *File_Name, char MSZ_File_Name)
{
		unsigned int scount;
		char i, j;

		if ( FCD_FAT160_Open_File(File_Name, MSZ_File_Name) != MX_ERR_FILENOTFOUND )
			return 0x01;													//File Already exists

		FCD_FAT160_MX_Format_File_String(File_Name, MSZ_File_Name);					//Convert filename to 8.3 format
		scount = 0;
		mx_current_address = mx_folder_start;								//Move to start of root
		while (scount < mx_folder_size)
		{
			FCD_FAT160_MX_Get_Buffer(mx_current_address);								//Read sector of root directory
			for(i=0;i<16;i++)											//16 file or directory references per sector
			{
				if (( FCD_FAT160_Read_Byte_From_Buffer(i * 32) == 0xe5) || ( FCD_FAT160_Read_Byte_From_Buffer(i * 32) == 0x00))		//Deleted or unused entry
				{
					for (j=0;j<11;j++)
					{
						FCD_FAT160_Write_Byte_To_Buffer((i * 32) + j, mx_name_ext[j]);
					}
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 11, 0x20);						//File Attribute
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 12, 0x18);						//Reserved for NT
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 13, 0);							//Creation time
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 14, MX_FILE_CREATETIME);
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 15, MX_FILE_CREATETIME >> 8);
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 16, MX_FILE_CREATEDATE);		//Creation date
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 17, MX_FILE_CREATEDATE >> 8);
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 18, MX_FILE_CREATEDATE);		//Access date
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 19, MX_FILE_CREATEDATE >> 8);
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 20, 0x00);						//Access time, must be 0 for Windows
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 21, 0x00);
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 22, MX_FILE_CREATETIME);		//Modified time 0 no modification entry
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 23, MX_FILE_CREATETIME >> 8);
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 24, MX_FILE_CREATEDATE);		//Modified date
					FCD_FAT160_Write_Byte_To_Buffer((i * 32) + 25, MX_FILE_CREATEDATE >> 8);

					for (j=26;j<32;j++)												//Sector location and length
					{
						FCD_FAT160_Write_Byte_To_Buffer((i * 32) + j, 0);
					}
					FCD_FAT160_MX_Send_Buffer(mx_current_address);
					return MX_ACK_OK;												//File created
				}
			}
		}
		return MX_ERR_ROOTFULL;														//No space left in root directory
}

char FCD_FAT160_Delete_File(char* File_Name, char MSZ_File_Name)
{
		unsigned int scount;
		char name_comp[11];
		unsigned int temp_mx_cluster;
		unsigned long fat_address;
		char i;

		FCD_FAT160_MX_Format_File_String(File_Name, MSZ_File_Name);						//Convert filename to 8.3 format
		scount = 0;
		mx_current_address = mx_folder_start;									//Move to start of root
		while (scount <= mx_folder_size)
		{
			FCD_FAT160_MX_Get_Buffer(mx_current_address);									//Read sector of root directory
			for(i=0;i<16;i++)													//16 file or directory references per sector
			{
				name_comp[0] = FCD_FAT160_Read_Byte_From_Buffer(0 + (i*32));							//Move DOS 8.3 Filename into 2nd comparison buffer
				name_comp[1] = FCD_FAT160_Read_Byte_From_Buffer(1 + (i*32));
				name_comp[2] = FCD_FAT160_Read_Byte_From_Buffer(2 + (i*32));
				name_comp[3] = FCD_FAT160_Read_Byte_From_Buffer(3 + (i*32));
				name_comp[4] = FCD_FAT160_Read_Byte_From_Buffer(4 + (i*32));
				name_comp[5] = FCD_FAT160_Read_Byte_From_Buffer(5 + (i*32));
				name_comp[6] = FCD_FAT160_Read_Byte_From_Buffer(6 + (i*32));
				name_comp[7] = FCD_FAT160_Read_Byte_From_Buffer(7 + (i*32));
				name_comp[8] = FCD_FAT160_Read_Byte_From_Buffer(8 + (i*32));
				name_comp[9] = FCD_FAT160_Read_Byte_From_Buffer(9 + (i*32));
				name_comp[10] = FCD_FAT160_Read_Byte_From_Buffer(10 + (i*32));

				if (FCI_COMPARE(mx_name_ext,11,name_comp,11,1) == 0)			//Perform filename comparison
				{
					mx_cluster = FCD_FAT160_Read_Byte_From_Buffer(26 + (i*32));						//Read Cluster of data start
					mx_cluster = mx_cluster | ((int) FCD_FAT160_Read_Byte_From_Buffer(27 + (i*32)) << 8);
					FCD_FAT160_Write_Byte_To_Buffer(26 + (i*32), 0);								//Overwrite mx_cluster value
					FCD_FAT160_Write_Byte_To_Buffer(27 + (i*32), 0);
					FCD_FAT160_Write_Byte_To_Buffer(28 + (i*32), 0);								//Clear file size
					FCD_FAT160_Write_Byte_To_Buffer(29 + (i*32), 0);
					FCD_FAT160_Write_Byte_To_Buffer(30 + (i*32), 0);
					FCD_FAT160_Write_Byte_To_Buffer(31 + (i*32), 0);
					FCD_FAT160_Write_Byte_To_Buffer(0 + (i*32), 0xe5);								//Set first byte to free file flag
					FCD_FAT160_MX_Send_Buffer(mx_current_address);							//Write the free file flag to the card

					while (mx_cluster >= 0x02 && mx_cluster <= 0xFFF8)			//While end of file not reached
					{
						if (mx_cluster > 0xFFF0)
							return 0xFE;										//Problem with FAT entry

						fat_address = mx_fat_start;								//Move to start of FAT
						while(mx_cluster >= 256)
						{
							fat_address = fat_address + 1;						//Move to next sector of FAT
							mx_cluster = mx_cluster - 256;
						}
						FCD_FAT160_MX_Get_Buffer(fat_address);								//Read FAT sector

						temp_mx_cluster = mx_cluster;
						mx_cluster = FCD_FAT160_Read_Byte_From_Buffer(temp_mx_cluster * 2);			//Read next mx_cluster value
						mx_cluster = mx_cluster | ((int) FCD_FAT160_Read_Byte_From_Buffer((temp_mx_cluster * 2) + 1) << 8);

						FCD_FAT160_Write_Byte_To_Buffer((temp_mx_cluster * 2), 0x00);				//Overwrite FAT entry
						FCD_FAT160_Write_Byte_To_Buffer(((temp_mx_cluster * 2) + 1), 0x00);

						FCD_FAT160_MX_Send_Buffer(fat_address);
						FCD_FAT160_MX_Send_Buffer(fat_address + mx_fat_size);				//Mirror data onto FAT 2
					}
					return 0;													//File deleted correctly
				}
			}
			scount = scount + 1;
			mx_current_address = mx_current_address + 1;
		}
		return MX_ERR_FILENOTFOUND;												//File not found
}

void FCD_FAT160_Write_Byte_To_Buffer(short Address, char Data)
{
		if (Address > 255)				//BufferH
		{
			Address = Address - 256;
			mx_card_bufferh[Address] = Data;
		}
		else							//BufferL
		{
			mx_card_bufferl[Address] = Data;
		}
}

char FCD_FAT160_Read_Byte_From_Buffer(short Address)
{
	char Data=0;
		if (Address > 255)						//BufferH
		{
			Address = Address - 256;
			Data = mx_card_bufferh[Address];
		}
		else
		{
			 Data = mx_card_bufferl[Address];	//BufferL
		}
	return (Data);
}

char FCD_FAT160_Open_Folder(char* Folder_Name, char MSZ_Folder_Name)
{
	
	// ".." - Up a level
	// "root" - Root directory
	// "foldername" - Down a level

		char i;
		unsigned int scount;
		char name_comp[11];

		//Root Directory
		if (Folder_Name[0] == 'r' && Folder_Name[1] == 'o' && Folder_Name[2] == 'o' && Folder_Name[3] == 't')
		{
			mx_folder_start = mx_root_start;		//Assign root start address to folder start
			mx_folder_size = mx_root_sectors;		//Assign root size to folder size
			return (0);								//Root Directory Specified
		}

		//Up a level
		if (Folder_Name[0] == '.' && Folder_Name[1] == '.')
		{
			mx_name_ext[0] = 0x2E;			// Assign search string with ".."
			mx_name_ext[1] = 0x2E;
			mx_name_ext[2] = 0x20;
			mx_name_ext[3] = 0x20;
			mx_name_ext[4] = 0x20;
			mx_name_ext[5] = 0x20;
			mx_name_ext[6] = 0x20;
			mx_name_ext[7] = 0x20;
			mx_name_ext[8] = 0x20;
			mx_name_ext[9] = 0x20;
			mx_name_ext[10] = 0x20;
		}

		else
			FCD_FAT160_MX_Format_File_String(Folder_Name, MSZ_Folder_Name);					//Format folder string to match 8 character folder name

		scount = 0;
		mx_current_address = mx_folder_start;										//Move to start of folder
		while (scount < mx_folder_size)
		{
			FCD_FAT160_MX_Get_Buffer(mx_current_address);										//Read sector of root directory
			for(i=0;i<16;i++)														//16 file or directory references per sector
			{
				name_comp[0] = FCD_FAT160_Read_Byte_From_Buffer(0 + (i*32));					//Move DOS 8.3 Filename into 2nd comparison buffer
				name_comp[1] = FCD_FAT160_Read_Byte_From_Buffer(1 + (i*32));
				name_comp[2] = FCD_FAT160_Read_Byte_From_Buffer(2 + (i*32));
				name_comp[3] = FCD_FAT160_Read_Byte_From_Buffer(3 + (i*32));
				name_comp[4] = FCD_FAT160_Read_Byte_From_Buffer(4 + (i*32));
				name_comp[5] = FCD_FAT160_Read_Byte_From_Buffer(5 + (i*32));
				name_comp[6] = FCD_FAT160_Read_Byte_From_Buffer(6 + (i*32));
				name_comp[7] = FCD_FAT160_Read_Byte_From_Buffer(7 + (i*32));
				name_comp[8] = FCD_FAT160_Read_Byte_From_Buffer(8 + (i*32));
				name_comp[9] = FCD_FAT160_Read_Byte_From_Buffer(9 + (i*32));
				name_comp[10] = FCD_FAT160_Read_Byte_From_Buffer(10 + (i*32));

				if (FCI_COMPARE(mx_name_ext,11,name_comp,11,1) == 0)				//Perform filename comparison
				{
					mx_cluster = FCD_FAT160_Read_Byte_From_Buffer(26 + (i*32));							//Read Cluster of folder start
					mx_cluster = mx_cluster | ((int) FCD_FAT160_Read_Byte_From_Buffer(27 + (i*32)) << 8);

					if (mx_cluster > 1)
					{
						mx_folder_start = mx_data_start + ((mx_cluster - 2) * 32);		//Assign folder start address
						mx_folder_size = 32;											//Assign folder size
					}
					else
					{
						mx_folder_start = mx_root_start;							//Assign root start address to folder start
						mx_folder_size = mx_root_sectors;							//Assign root size to folder size
					}
					return MX_ACK_OK;												//Folder found and moved
				}
			}
			mx_current_address = mx_current_address + 1;
			scount = scount + 1;													//Move to next sector of folder
		}
		return MX_ERR_FILENOTFOUND;													//Folder not found
}

short FCD_FAT160_Scan_Current_Folder(short Current_Idx, char Scan_Type)
{
		char i;

		if (Current_Idx < (mx_folder_size * 16))
		{
			mx_current_address = mx_folder_start;							//Move to start of folder
			while(Current_Idx >= 16)										//While address is not in the current sector
			{
				Current_Idx = Current_Idx - 16;								//Modify address value
				mx_current_address = mx_current_address + 1;				//Increment sector address
			}

			FCD_FAT160_MX_Get_Buffer(mx_current_address);								//Read sector of root directory

			if (( FCD_FAT160_Read_Byte_From_Buffer(Current_Idx  * 32) == 0xe5) || ( FCD_FAT160_Read_Byte_From_Buffer(Current_Idx  * 32) == 0x00) || ( FCD_FAT160_Read_Byte_From_Buffer((Current_Idx * 32) + 2) == 0x00))		//Deleted or unused entry
				return MX_ACK_FILEEMPTY;									//Address is empty

			if(Scan_Type)	//scan for folder
			{
				if (( FCD_FAT160_Read_Byte_From_Buffer((Current_Idx  * 32) + 11) & 0x10) == 0)	//If a file has been found
					return MX_ACK_FILETYPE;									//Address in use but incorrect data type
			}
			else			//scan for file
			{
				if (( FCD_FAT160_Read_Byte_From_Buffer((Current_Idx  * 32) + 11) & 0x20) == 0)	//If a folder has been found
					return MX_ACK_FILETYPE;									//Address in use but incorrect data type
			}

			for (i=0;i<11;i++)
				mx_name_ext[i] = FCD_FAT160_Read_Byte_From_Buffer((Current_Idx * 32) + i);		//Read string from address

			if(Scan_Type == 0)												//If scanning for a file then rearrange 8.3 filename back into string
			{
				for (i=0;i<7;i++)
				{
					if (mx_name_ext[i] == 0x20)								//If a padding space has been found
						break;
				}
				mx_name_ext[i] = '.';										//Add the '.' seperator to the filename
				mx_name_ext[i + 1] = FCD_FAT160_Read_Byte_From_Buffer((Current_Idx * 32) + 8);
				mx_name_ext[i + 2] = FCD_FAT160_Read_Byte_From_Buffer((Current_Idx * 32) + 9);
				mx_name_ext[i + 3] = FCD_FAT160_Read_Byte_From_Buffer((Current_Idx * 32) + 10);
				for (i=i+4;i<12;i++)
					mx_name_ext[i] = ' ';
			}
			return MX_ACK_OK;												//File/folder found and stored into string array
		}
		return MX_ACK_EOF;													//End of folder reached

}

char FCD_FAT160_Read_Byte_From_Scan(char idx)
{
		if(idx > 11)
			return 0;				//String buffer 12 chars long max
		return (mx_name_ext[idx]);	//Return character from string
}

char FCD_FAT160_MX_SPI_BYTE(char shift)
{
	
			char idx;
			for (idx = 0; idx < 8; idx++)
			{
				if (shift & 0b10000000)
					output_high(MX_CARD_SDO);					// Set SDO bit
				else
					output_low(MX_CARD_SDO);					// Clear SDO bit

				shift = shift << 1;
				output_high(MX_CARD_SCK);						// SCK Output 1
				if(input(MX_CARD_SDI)) 
					shift = shift | 0x01;
				else
					shift = shift & 0xFE;

				output_low(MX_CARD_SCK);						// SCK Output 0
			}
			return shift;
}

char FCD_FAT160_MX_Init_Card()
{
		char RetVal;
		short i;

		//Configure Port Pin Data Direction
		output_drive(MX_CARD_CS); 					// CS Output
		output_drive(MX_CARD_SCK);  				// SCK Output
		output_float(MX_CARD_SDI);  				// SDI Input
		output_drive(MX_CARD_SDO);  				// SDO Output

		//Configure Port Pin Default Output
		output_high(MX_CARD_SCK); 					// SCK Disable
		output_high(MX_CARD_SDO); 					// SDO Disable
		output_high(MX_CARD_CS); 					// CS Disable

		delay_ms(50);							//Wait for card to power up

		output_high(MX_CARD_CS);				//CARD-Disabled
		for(i = 0; i < 10; i++)					//Start CARD in SPI mode
		{
			FCD_FAT160_MX_SPI_BYTE(0xFF);		//Send 10*8=80 clock pulses
		}
		output_low(MX_CARD_CS);					//CARD-Enabled
		delay_us(50);							//Wait 50us

		//Command 0 - Goto Idle State - Resets card
		FCD_FAT160_MX_SPI_BYTE(0x40);						//Send Command 0
		FCD_FAT160_MX_SPI_BYTE(0x00);						//Arg3
		FCD_FAT160_MX_SPI_BYTE(0x00);						//Arg2
		FCD_FAT160_MX_SPI_BYTE(0x00);						//Arg1
		FCD_FAT160_MX_SPI_BYTE(0x00);						//Arg0
		FCD_FAT160_MX_SPI_BYTE(0x95);						//CRC
		FCD_FAT160_MX_SPI_BYTE(0xFF);
		RetVal = FCD_FAT160_MX_SPI_BYTE(0xFF);				//Wait for response of 1
		if (RetVal != 1)
			return MX_ERR_NOCARD;				//Card not initialising or not present

		output_high(MX_CARD_CS);				//CARD-Disabled
		delay_us(50);							//Wait 50us
		output_low(MX_CARD_CS);					//CARD-Enabled - Command 0 complete

		//Command 1 - Send Operating Condition - Reports the status of the card
		i = 512;
		while ((RetVal != 0) && (i))			//Loop while processing command 1
		{
			i = i - 1;
			output_high(MX_CARD_CS);				//CARD-Disabled
			FCD_FAT160_MX_SPI_BYTE(0xFF);
			FCD_FAT160_MX_SPI_BYTE(0xFF);
			output_low(MX_CARD_CS);					//CARD-Enabled - Command 0 complete
			FCD_FAT160_MX_SPI_BYTE(0x41);					//Send Command 1
			FCD_FAT160_MX_SPI_BYTE(0x00);					//Arg3
			FCD_FAT160_MX_SPI_BYTE(0x00);					//Arg2
			FCD_FAT160_MX_SPI_BYTE(0x00);					//Arg1
			FCD_FAT160_MX_SPI_BYTE(0x00);					//Arg0
			FCD_FAT160_MX_SPI_BYTE(0xFF);					//CRC
			FCD_FAT160_MX_SPI_BYTE(0xFF);
			RetVal = FCD_FAT160_MX_SPI_BYTE(0xFF);			//Check Response
		}
		output_high(MX_CARD_CS);					//CARD-Disabled
		if (i == 0)
			return MX_ERR_NORESP;				//Card not responding

		return MX_ACK_OK;						//Card Ready
}

char FCD_FAT160_MX_Send_Buffer(long address)
{
		int idx;
		char retval;

		address = address << 9;

		output_high(MX_CARD_CS);					// Card Disable
		FCD_FAT160_MX_SPI_BYTE(0xFF); 						//Padd Command
		output_low(MX_CARD_CS);						// Card Enable
		FCD_FAT160_MX_SPI_BYTE(0x58);  					//Send command
		FCD_FAT160_MX_SPI_BYTE(((long) address >> 24) & 0xFF);	//Send MSB Of Address
		FCD_FAT160_MX_SPI_BYTE(((long) address >> 16) & 0xFF);
		FCD_FAT160_MX_SPI_BYTE((address >> 8) & 0xFF);
		FCD_FAT160_MX_SPI_BYTE(address & 0xFF);			//Send LSB Of Address
		FCD_FAT160_MX_SPI_BYTE(0xFF);  					//Skip CRC Check
		idx = 1024;
		retval = FCD_FAT160_MX_SPI_BYTE(0xFF);
		while ((retval == 0xFF) && idx)
		{
			retval = FCD_FAT160_MX_SPI_BYTE(0xFF);  		//Wait for card
			idx = idx - 1;
		}
		if (idx == 0)
		{
			output_high(MX_CARD_CS);
			return MX_ERR_NOACK;				//Command not acknowledged
		}

		FCD_FAT160_MX_SPI_BYTE(0xFE);						//Send Start Byte
		for (idx=0; idx < 256; idx++)			//Write Low Buffer
		{
			FCD_FAT160_MX_SPI_BYTE(mx_card_bufferl[idx]);
		}
		for (idx=0; idx < 256; idx++)			//Write High Buffer
		{
			FCD_FAT160_MX_SPI_BYTE(mx_card_bufferh[idx]);
		}
		FCD_FAT160_MX_SPI_BYTE(0xFF);								//Send 2 dummy bytes to skip CRC
		FCD_FAT160_MX_SPI_BYTE(0xFF);
		idx = 10000;
		retval = FCD_FAT160_MX_SPI_BYTE(0xFF);						//Check if CARD is still busy
		while((retval != 0xFF) && idx)
		{
			retval = FCD_FAT160_MX_SPI_BYTE(0xFF);					//Wait until CARD is not busy anymore
			idx = idx - 1;
		}
		if (idx == 0)
		{
			output_high(MX_CARD_CS);
			return MX_ERR_TOUT;					//Read timed out
		}
		output_high(MX_CARD_CS);
		return MX_ACK_OK;						//Write Complete
}

char FCD_FAT160_MX_Get_Buffer(unsigned long address)
{
		short idx;
		int CRC;
		char temp;
		unsigned int count;

		address = address << 9;

		FCD_FAT160_MX_Clear_Buffer();
		output_low(MX_CARD_CS);

		FCD_FAT160_MX_SPI_BYTE(0xFF);							//Send Dummy Byte
		FCD_FAT160_MX_SPI_BYTE(0x51);							//Send Read Command
		FCD_FAT160_MX_SPI_BYTE(((long) address >> 24) & 0xFF);		//Send MSB Of Address
		FCD_FAT160_MX_SPI_BYTE(((long) address >> 16) & 0xFF);
		FCD_FAT160_MX_SPI_BYTE((address >> 8) & 0xFF);
		FCD_FAT160_MX_SPI_BYTE(address & 0xFF);				//Send LSB Of Address
		FCD_FAT160_MX_SPI_BYTE(0xFF);							//CRC unused

		temp = 0xFF;
		count = 10;
		while((temp == 0xFF) && count)
		{
			temp = FCD_FAT160_MX_SPI_BYTE(0xFF);				//Wait for response
			count = count - 1;
		}
		if (count == 0)
		{
			output_high(MX_CARD_CS);
			return MX_ERR_NOACK;					//Read Not Acknowledged
		}

		temp = 0;
		count = 65000;
		while ((temp != 0xFE) && count)			//Wait for data ready
		{
			temp = FCD_FAT160_MX_SPI_BYTE(0xFF);
			delay_us(10);
			count = count - 1;
		}
		if (count == 0)
			return MX_ERR_TOUT	;					//Read Timed Out

		for (idx=0; idx < 256; idx++)				//Read Low Buffer
		{
			mx_card_bufferl[idx] = FCD_FAT160_MX_SPI_BYTE(0xFF);
		}

		for (idx=0; idx < 256; idx++)				//Read High Buffer
		{
			mx_card_bufferh[idx] = FCD_FAT160_MX_SPI_BYTE(0xFF);
		}
		CRC = FCD_FAT160_MX_SPI_BYTE(0xFF);					//Read 16-Bit CRC
		CRC = CRC << 8;
		CRC = CRC | FCD_FAT160_MX_SPI_BYTE(0xFF);

		Output_high(MX_CARD_CS);
		FCD_FAT160_MX_SPI_BYTE(0xFF);							//Trailing byte
		return MX_ACK_OK;							//Read Complete
}

void FCD_FAT160_MX_Format_File_String(char* File_Name, char MSZ_File_Name)
{
		char i;
		for (i=0;i<11;i++)
			mx_name_ext[i] = 0x20;											//Fill name variable with spaces

		FCI_TOUPPER(File_Name,MSZ_File_Name,File_Name,MSZ_File_Name);	//convert to uppercase

		for (i=0; i<8 && *File_Name != '.' && *File_Name; i++)			//Copy file name to comparison variable
			mx_name_ext[i] = *File_Name++;

		if(*File_Name == '.')											//Find the '.' seperator
		{
			File_Name++;												//and then shift the extension
			for(i=8;i<11 && *File_Name; i++)
				mx_name_ext[i] = *File_Name++;
		}
}

void FCD_FAT160_MX_Add_Sector_To_File()
{
		unsigned int idx_fat;
		unsigned int fat_pointer = 0xFFFF;
		unsigned long fat_address;

		idx_fat = 1;
		fat_address = mx_fat_start;							//Move to start of FAT
		FCD_FAT160_MX_Get_Buffer(fat_address);							//Read FAT sector
		while (fat_pointer > 0)								//Scan FAT for free entry
		{
			idx_fat = idx_fat + 1;							//Increment FAT address - Start at address 2
			if(idx_fat == 256)								//If at the end of the FAT sector
			{
				fat_address = fat_address + 1;
				FCD_FAT160_MX_Get_Buffer(fat_address);					//Read next FAT sector
				idx_fat = 0;
			}
			fat_pointer = FCD_FAT160_Read_Byte_From_Buffer(idx_fat * 2);			//Extract data mx_cluster address
			fat_pointer = fat_pointer | ((int) FCD_FAT160_Read_Byte_From_Buffer((idx_fat * 2) + 1) << 8);
		}

		FCD_FAT160_Write_Byte_To_Buffer((idx_fat * 2), 0xFF);					//Write end of file marker into new FAT address
		FCD_FAT160_Write_Byte_To_Buffer(((idx_fat * 2) + 1), 0xFF);
		FCD_FAT160_MX_Send_Buffer(fat_address);
		FCD_FAT160_MX_Send_Buffer(fat_address + mx_fat_size);				//Mirror data onto FAT 2

		fat_address = ((fat_address - mx_fat_start) * 256) + idx_fat;
		idx_fat = mx_cluster;
		mx_cluster = fat_address & 0xFFFF;						//Store new mx_cluster address

		fat_address = mx_fat_start;
		while (idx_fat >= 256)
		{
			idx_fat = idx_fat - 256;
			fat_address = fat_address + 1;
		}
		FCD_FAT160_MX_Get_Buffer(fat_address);
		FCD_FAT160_Write_Byte_To_Buffer((idx_fat * 2), mx_cluster);				//Update old FAT address
		FCD_FAT160_Write_Byte_To_Buffer(((idx_fat * 2) + 1), (mx_cluster >> 8));
		FCD_FAT160_MX_Send_Buffer(fat_address);
		FCD_FAT160_MX_Send_Buffer(fat_address + mx_fat_size);				//Mirror data onto FAT 2

		mx_file_sector = mx_file_sector + 1;						//Increment file sector
		mx_current_address = mx_data_start + ((mx_cluster - 2) * 32);//Move to next sector in file
		FCD_FAT160_MX_Get_Buffer(mx_current_address);
}

void FCD_FAT160_MX_Clear_Buffer()
{
		short count;

		for (count = 0; count < 256; count++)
		{
			mx_card_bufferh[count] = 0x00;
			mx_card_bufferl[count] = 0x00;
		}
}

char FCD_FAT160_Read_Next_File_Sector()
{
		unsigned int temp_mx_cluster;
		unsigned int old_mx_cluster;
		unsigned long fat_address;
		char retval;

		mx_current_address = mx_current_address + 1;								//Increment address
		if (((mx_current_address - mx_data_start) % 0x20) == 0)						//If end of mx_cluster reached then find next mx_cluster
		{
			temp_mx_cluster = mx_cluster;
			fat_address = mx_fat_start;												//Move to start of FAT
			while (temp_mx_cluster >= 256)											//256 FAT entries per sector
			{
				temp_mx_cluster = temp_mx_cluster - 256;
				fat_address = fat_address + 1;										//Increment sector of FAT
			}
			FCD_FAT160_MX_Get_Buffer(fat_address);												//Read FAT sector
			old_mx_cluster = mx_cluster;
			mx_cluster = FCD_FAT160_Read_Byte_From_Buffer(temp_mx_cluster * 2);							//Extract data mx_cluster address
			mx_cluster = mx_cluster | ((int) FCD_FAT160_Read_Byte_From_Buffer((temp_mx_cluster * 2) + 1) << 8);

			if (mx_cluster == 0 || mx_cluster >= 0xFFF0)							//If not an address
			{
				if (mx_cluster > 0xFFF7)
					retval = MX_ACK_EOF;											//End of file
				if (mx_cluster < 0xFFF7)
					retval = MX_ERR_RESERVED;										//Reserved mx_cluster
				if (mx_cluster == 0xFFF7)
					retval = MX_ERR_BAD;											//Bad mx_cluster
				if (mx_cluster == 0)
					retval = MX_ERR_FREE;											//Free mx_cluster - Fat Error

				mx_current_address = mx_current_address - 1;						//Restore address
				mx_cluster = old_mx_cluster;										//Restore mx_cluster
				FCD_FAT160_MX_Get_Buffer(mx_current_address);									//Read sector
				return retval;
			}

			mx_current_address = mx_data_start + ((mx_cluster - 2) * 32);			//Move to next sector in file
		}

		mx_file_sector = mx_file_sector + 1;										//Increment file sector

		return FCD_FAT160_MX_Get_Buffer(mx_current_address);									//Read sector
}

