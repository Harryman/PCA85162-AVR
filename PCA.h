/*
  PCA85162.h - 
  Uses the Wire.h library
  
  Created by Neal Meeker, February 25, 2013.
  Created for TEC (The Everything Corp, LLC)
*/
#ifndef PCA_h
#define PCA_h

#include <stdint.h>


#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>



//############################################# PCA Chip Settings ############################################################
//Command Modes *************************************************
//Continuation Bit
#define Cont 0b10000000 //flag 1 means another command is to follow
//***************************************************************

//Mode Set C10-EB01 #############################################
// Command defines x000xx00
#define DISPLAY_ENABLE 0b00001000  //Enables

#define LCD_BIAS 0b00000100  //default off=1/3 bias, on=1/2 bias

#define LCD_MODE_COMMAND 0b01000000
#define LCD_MODE_SET_STATIC 0b00000001  //Most applications will be set static
#define LCD_MODE_SET_2MULTIPLEX 0b00000010
#define LCD_MODE_SET_3MULTIPLEX 0b00000011
#define LCD_MODE_SET_4MULTIPLEX 0b00000000
//Wire.write(Cont | LCD_MODE_COMMAND | DISPLAY_ENABLE | LCD_BIAS | LCD_MODE_SET_STATIC);
//##############################################################

//DATA_POINTER .... position 0-31 ^^^^^^^^^^^^^^^^^^^^
#define DATA_POINTER_0 0b00000000 
#define DATA_POINTER_7 0b00000111
#define DATA_POINTER_14 0b00001110
#define DATA_POINTER_21 0b00010101
#define DATA_POINTER_28 0b00011100
#define DATA_POINTER_29 0b00011101
#define DATA_POINTER_30 0b00011110
#define DATA_POINTER_31 0b00011111
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//DEVICE_SELECT &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
#define LCD_DEVICE_SELECT 0b01100000 //Default one device
//can change last three bits to choose one of 8 hardware subaddresses
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//BANK_SELECT %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//No effect in 1:3 or 1:4 multiplex drive modes
#define RAM_BANK_SELECT 0b01111000 //default = input row 0 and output row 0
#define INPUT_BANK 0b00000010 //row 2 input... WRITE TO RAM ROW 2
#define OUTPUT_BANK 0b00000001 //row 2 output... DISPLAY FROM ROW 2
//Wire.write(Cont | RAM_BANK_SELECT | INPUT_BANK);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//BLINK_SELECT @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define BLINK_SELECT 0b01110000 //default ... normal blink... frequency off
#define ALTERNATE_BLINK 0b00000100 //Alternate RAM bank switching

#define FREQUENCY_BLINK_0 0b00000000
#define FREQUENCY_BLINK_1 0b00000001 //Rabid Blink... about 4x per sec
#define FREQUENCY_BLINK_2 0b00000010 //Medium Blink... about .5 seconds
#define FREQUENCY_BLINK_3 0b00000011 //Slowest Blink.. about 1 second
//Wire.write(BLINK_SELECT | ALTERNATE_BLINK | FREQUENCY_BLINK_3);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#define MID 0b0000000011000000
#define TL  0b0000000000110000
#define TOP 0b0000000000001100
#define TR  0b0000000000000011
#define BR  0b1100000000000000 
#define BOT 0b0011000000000000 
#define BL  0b0000110000000000 
#define DECI 0b0000001100000000 


#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

//#############################################  END PCA Chip Settings ########################################################




class PCA
{
  public:
    PCA();
    void start();
	void start(byte i2c_addr);
    void blink(uint8_t i);
    void drawColon(boolean state);
    void updateDisplay();
	void clearBuff();
	void print(char *str);
	void print(char *str, int pos);
	void print(char c);
	void print(char c, int pos);
	void print(unsigned int ui);
	void print( unsigned int ui, int pos);
	void print(int i);
	void println(int i);
	void split(int i, int pos);
	void print(int i, int pos);
	void print(long i);
	void print(long i,int pos);
	void print(unsigned long i);
	void print(unsigned long i,int pos);
	void print(float f, int digits);
	void print(float n);
	void print(double n);
	void print(double n, int digits);
	void printFloat(double number, uint8_t digits);
  private:
    byte _i2c_addr;
    uint8_t posit;
    uint16_t displayBuff[8]; 
	boolean colon;
};

#endif

