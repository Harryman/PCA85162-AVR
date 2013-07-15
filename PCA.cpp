/*
  PCA85162.h - 
  Uses the Wire.h library
  
  Created by Neal Meeker, February 25, 2013.
  Created for TEC (The Everything Corp, LLC)
*/

#include <stdint.h>
#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>
#include "PCA.h"

//############################### Alfa Num Table #######################################
//void PCA::setTable(){
 uint16_t alphaNumTable[] = {
  0x0000, /* space 32*/
  0x3C3C, /* ( 40*/
  0xF00F, /* ) 41*/
  0x00C0, /* - 45 */
  //0x3000, /* . 46 set a different way... */
  0x0CC3, /* / 47*/


  0xFC3F, /* 0 */ 
  0xC003, /* 1 */
  0x3CCF, /* 2 */
  0xF0CF, /* 3 */
  0xC0F3, /* 4 */
  0xF0FC, /* 5 */
  0xFCFC, /* 6 */
  0xC00F, /* 7 */
  0xFCFF, /* 8 */
  0xF0FF, /* 9 */

  0xCCFF, /* a 65*/
  0xFCF0, /* b */
  0x3C3C, /* C */
  0xFCC3, /* d */
  0x3CFC, /* E */
  0x0CFC, /* F */
  0xF0FF, /* g */
  0xCCF3, /* h */
  0x0C30, /* i */
  0xFC03, /* j */
  0x3000, /* k.. No K */
  0x3C30, /* L */
  0x3000, /* M.. No M */
  0xCCC0, /* N */
  0xFC3F, /* O */
  0x0CFF, /* P */
  0x3000, /* q... No Q.. */
  0x0CC0, /* r */
  0xF0FC, /* s */
  0x3000, /* t.. no t */
  0xFC33, /* u */
  0xFC00, /* v... Same as v? */
  0x3000, /* w... No W... */
  0x3000, /* x.. No X */
  0xF0F3, /* y */
  0x3CCF, /* z 90*/
  0x3C3C, /* [ 91*/
  0xC003 , /* \ ... shouldn't print.. 92*/
  0xF00F, /* ] 93*/
  0x3000, /*  94*/
  0x3000, /* _ 95*/
  0x0C30 /* | 124*/
};
//############################### Alfa Num Table #######################################
//####################################### Setting up i2c #######################
//Starts and sets up PCA chip to work with screen.
PCA::PCA()
{
  //PCA(0b00111000);  //defaul I2C address
}
void PCA::start()
{
  start(0b00111000);
}
void PCA::start(byte i2c_addr)
{
  _i2c_addr = i2c_addr;  //Sets address for use later
  //setTable();
  posit = 0;
  
  delay(2); //PCA needs to reset before initializing I2C.. takes a millesecond or 2
  Wire.begin();
  delay(2);
  Wire.beginTransmission(_i2c_addr);
  Wire.write(LCD_MODE_COMMAND | DISPLAY_ENABLE | LCD_MODE_SET_2MULTIPLEX); 
  //Chip Settings for Command Mode: Enabled and 2xMultiplex (DEFAULT)
  //Sending this command to PCA chip sets the chip to receive data and display correctly
  Wire.endTransmission();
}
//####################################### End Setting up i2c #######################


//############################################ Blink Mode ##############################
  //PCA.Blink(1); = Fast Blink, 2 = Medium, 3 = Slow, 0 turns off screen, anything else turns off blink mode
void PCA::blink(uint8_t i)
{
  Wire.beginTransmission(_i2c_addr);
  if(i < 4) 
  {
    Wire.write(Cont | BLINK_SELECT | ALTERNATE_BLINK | i); //Turn on Blink
  }
  else
  {
    Wire.write(((Cont | BLINK_SELECT) | ALTERNATE_BLINK) & 3); //Turn off Blink
  }
  Wire.endTransmission();
}
//######################################## End Blink Mode ##############################


//############################################ Draw Colon: #############################
//Turn Colon on or off 
void PCA::drawColon(boolean state) {
  if (state)
    displayBuff[3] = displayBuff[3] | DECI;
  else
    displayBuff[3] = displayBuff[3] &= ~DECI;
}
//######################################## End Draw Colon: #############################


//########################################## Update Display ############################
//Updates the display to show the newest information
void PCA::updateDisplay(void){
  Wire.beginTransmission(_i2c_addr);
  Wire.write(0x00); //Set DataPointer to 0
  if (colon == 1){
  drawColon(1);
  colon = 0;
  }
  else {
  drawColon(0);
  }
  
  for (uint8_t i=0; i<4; i++) {
    uint16_t  tx = displayBuff[i];
    Wire.write((unsigned char *)&tx, 2);
  }
  Wire.endTransmission();
  posit = 0;
}
//###################################### End Update Display ############################


//######################################### Clear Display Buffer #######################
void PCA::clearBuff(void) {
  for (uint8_t i=0; i<4; i++) {
    displayBuff[i] = 0;
  }
}
//##################################### End Clear Display Buffer #######################


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//##################################### Printing Char ##################################
void PCA::print(char *str){
  int x;
  for (x = 0; str[x] != '\0'; x++)
  {
    print(str[x], posit);
  }
}

void PCA::print(char *str, int pos){
  int x;
  for (x = 0; str[x] != '\0'; x++)
  {
    print(str[x], pos);
    pos += 1;
  }
}
void PCA::print(char c){
	print(c, posit);
}
void PCA::print(char c, int pos){
  if (c == 46){
    if (pos < 4){
    displayBuff[posit - 1] = displayBuff[posit - 1] | DECI;
    }
  }   
   else if(c == 32){
    displayBuff[pos] = alphaNumTable[c - 32];
    posit = pos + 1;
  }
   else if(c == 58){
    colon = 1;
  }
   else if (c == 40){
    displayBuff[pos] = alphaNumTable[c - 39];
    posit = pos + 1;
  }
    else if (c == 41){
    displayBuff[pos] = alphaNumTable[c - 39];
    posit = pos + 1;
  }
    else if (c == 45){
    displayBuff[pos] = alphaNumTable[c - 42];
    posit = pos + 1;
  } 
  else if ((c > 46) & (c < 58)){
    displayBuff[pos] = alphaNumTable[c - 43];
    posit = pos + 1;
  }
  else if((c > 64) & (c < 96)){
    displayBuff[pos] = alphaNumTable[c - 50];
    posit = pos + 1;
  }
  else if((c > 96) & (c < 126)){
    displayBuff[pos] = alphaNumTable[c - 82];
    posit = pos + 1;
  }  
  else{
    displayBuff[pos] = alphaNumTable[45];
    posit = pos + 1;
  }
}
//################################### End Printing Char ################################

//#################################### Print Num #######################################
void PCA::print(unsigned int ui){
  print((int) ui);
}
void PCA::print( unsigned int ui, int pos){
  print((int) ui, pos); 
}

void PCA::print(int i){
// Handle negative numbers
  if (i < 0.0)
  {
    print('-');
    i = -i;
  }
  split(i,posit); 
}
void PCA::println(int i){
  int a = i / 1000;
  int  b =  i % 1000;
  b = b / 100;
  int  c = i % 100;
  c = c / 10;
  int  d = i % 10;
  print(a,0);
  print(b,1);
  print(c,2);
  print(d,3);
}

// Split breaks the integer into single digits
void PCA::split(int i, int pos){
  int arry[4];
  int posi = pos;
  
  int a = i / 1000;
  int  b =  i % 1000;
  b = b / 100;
  int  c = i % 100;
  c = c / 10;
  int  d = i % 10;
  arry[0] = d;
  arry[1] = c;
  arry[2] = b;
  arry[3] = a;
  boolean ready = false;
  for (int x = 3; x >= 0; x--)
  {
    if (arry[x] > 0){
      ready = true;
    }
    if (ready == true){
      print(arry[x],posi);
      posi++;
    }


  }
}

void PCA::print(int i, int pos){
  if (i < 10){
    displayBuff[pos] = alphaNumTable[i + 5];
    posit = pos + 1;
  }
  else{
    split(i, pos); 
  }
}
void PCA::print(long i){
  print((int) i);
}
void PCA::print(long i,int pos){
  print((int) i, pos);
}
void PCA::print(unsigned long i){
  print((int) i);
}
void PCA::print(unsigned long i,int pos){
  print((int) i, pos);
}

// digits would be the number of decimal places to round to
void PCA::print(float f, int digits){
  printFloat((double) f, digits); 
}
void PCA::print(float n){
  printFloat((double) n, 2); //defaulted 2 decimal points
}
void PCA::print(double n){
  printFloat(n,2);
}
void PCA::print(double n, int digits)
{
  printFloat(n, digits);
}

//printFloat is used to handle floats & doubles
void  PCA::printFloat(double number, uint8_t digits) 
{ 
  // Handle negative numbers
  if (number < 0.0)
  {
    print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned int int_part = (unsigned int)number;
  double remainder = number - (double)int_part;
  print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    print('.'); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    print(toPrint);
    remainder -= toPrint; 
  } 
}
//################################# End Print Num ##########################################





