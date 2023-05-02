////////////////////////////////////////////////////////////////////////////
////                                                                    ////
////                 Driver for common I2C modules                      ////
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

//-------------------------xxx------------------------

void main();
signed byte  lecture_i2c(byte device, byte address);
void ecriture_i2c(byte device, byte address, byte data);


//---------------------Lecture BUS I2C--------------------------
signed byte Lecture_I2C(byte device, byte address) {
   signed BYTE data;

   i2c_start();
   i2c_write(device);
   i2c_write(address);
   i2c_start();
   i2c_write(device | 1);
   data=i2c_read();
   i2c_stop();
   return(data);
}

//---------------------Lecture BUS I2C--------------------------
signed int16 Lecture2b_I2C(byte device, byte address) {
	byte data1,data2;
	signed int16 data;

   i2c_start();
   i2c_write(device);
   i2c_write(address);
   i2c_start();
   i2c_write(device | 1);
   data|=data2;						// MSB
   data<<=8;
   data|=data1;						//LSB
   i2c_stop();
   return(data);
}

//--------------------Ecriture BUS I2C------------------------
void Ecriture_I2C(byte device, byte address, byte data) {
   i2c_start();
   i2c_write(device);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
}

//--------------------Ecriture Commande BUS I2C------------------------
void Wr_Cmd_I2C(byte device, byte cmd) {
   i2c_start();
   i2c_write(device);
   i2c_write(cmd);
   i2c_stop();
}
