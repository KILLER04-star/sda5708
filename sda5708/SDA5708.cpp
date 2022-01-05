/**************************************************************
 *      Library for Display SDA5707
 *      ===========================
 *           Uwe Berger; 2009
 *
 * 20131101, randy:
 *  - rewrite for arduino IDE (http://blog.smue.org/archives/433-DBOX1-SDA5708-display-+-arduino.html)
 * 20150710, Felix Wiedemann:
 *  - made Arduino Module, translated
 *
 * --> Hardware protocol description of the display SDA5708:
 * Source: http://www.sbprojects.com/knowledge/footprints/sda5708/index.php
 *
 *20210412, Paul Neumann:
 * - add SDA5708.clear() function
 * - add ability to set cursor
 *
 *
 * SDA5708 Pins (Pin 1 is right of the back side)
 * -------------------------------------------------
 * Pin Function
 * 1   Vcc
 * 2   LOAD
 * 3   DATA
 * 4   SDCLK
 * 5   RESET
 * 6   GND
 *
 *
 * SDA5708 Datagram
 * ----------------
 *
 * LOAD  ---+                          +---
 *          |                          |
 *          +--------------------------+
 * DATA  ----+----+----+----+----+----+----+----+----+-----
 *           | D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7 |
 *       ----+----+----+----+----+----+----+----+----+-----
 * SDCLK --+    +--+ +--+ +--+ +--+ +--+ +--+ +--+ +--+
 *         |    |  | |  | |  | |  | |  | |  | |  | |  |
 *         +----+  +-+  +-+  +-+  +-+  +-+  +-+  +-+  +-----
 *
 *
 * SDA5708 Register
 * ----------------
 *
 * SDA5708 --> Control-Register
 * 76543210
 * ||||||||
 * |||||000 --> 100% Brightness
 * |||||001 --> 53%
 * |||||010 --> 40%
 * |||||011 --> 27%
 * |||||100 --> 20%
 * |||||101 --> 13%
 * |||||110 --> 6.6%
 * |||||111 --> 0% (dark ;-))
 * ||||+------> 0=max. peak current; 1=12.5% of max. peak current
 * |||0
 * ||+--------> 0=clear display; 1=normal
 * 11
 *
 *
 * SDA5807 --> Adress-Register (Digit 0 left; Digit 7 right)
 * 76543210
 * ||||||||
 * |||||000 --> Digit 0
 * |||||001 --> Digit 1
 * |||||010 --> ...
 * |||||111 --> Digit 7
 * 10100
 *
 * ---------
 * Have fun!
 *
 ***************************************************************/
#include "Arduino.h"
#include "SDA5708.h"
#include <avr/pgmspace.h>

const unsigned char font[752] PROGMEM ={

   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,    //letter Space


   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00000000,
   0b00100000,    //letter !

   0b01010000,
   0b01010000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,    //letter "

   0b01010000,
   0b01010000,
   0b11111000,
   0b01010000,
   0b11111000,
   0b01010000,
   0b01010000,    //letter #

   0b00100000,
   0b01111000,
   0b10100000,
   0b01110000,
   0b00101000,
   0b00110000,
   0b00100000,    //letter $

   0b11000000,
   0b11001000,
   0b00010000,
   0b00100000,
   0b01000000,
   0b10011000,
   0b00011000,    //letter %


   0b01000000,
   0b10100000,
   0b01000000,
   0b10100000,
   0b10010000,
   0b10001000,
   0b01110000,    //letter &

   0b00010000,
   0b00010000,
   0b00100000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,    //letter '

   0b00100000,
   0b01000000,
   0b01000000,
   0b01000000,
   0b01000000,
   0b01000000,
   0b00100000,    //letter (

   0b00010000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00010000,    //letter )

   0b00000000,
   0b10001000,
   0b01010000,
   0b11111000,
   0b01010000,
   0b10001000,
   0b00000000,    //letter *

   0b00000000,
   0b00100000,
   0b00100000,
   0b11111000,
   0b00100000,
   0b00100000,
   0b00000000,    //letter +

   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b01000000,
   0b01000000,
   0b10000000,    //letter ,

   0b00000000,
   0b00000000,
   0b00000000,
   0b11111000,
   0b00000000,
   0b00000000,
   0b00000000,    //letter -

   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b11000000,
   0b11000000,   //letter .

   0b00000000,
   0b00001000,
   0b00010000,
   0b00100000,
   0b01000000,
   0b10000000,
   0b00000000,   //letter /

   0b01110000,
   0b10001000,
   0b10011000,
   0b10101000,
   0b11001000,
   0b10001000,
   0b01110000,   //letter 0

   0b00100000,
   0b01100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b01110000,   //letter 1

   0b01110000,
   0b00001000,
   0b00001000,
   0b01110000,
   0b10000000,
   0b10000000,
   0b11111000,   //letter 2

   0b11110000,
   0b00001000,
   0b00001000,
   0b01110000,
   0b00001000,
   0b00001000,
   0b11110000,   //letter 3

   0b00001000,
   0b00011000,
   0b00101000,
   0b01001000,
   0b11111000,
   0b00001000,
   0b00001000,   //letter 4

   0b11111000,
   0b10000000,
   0b10000000,
   0b11110000,
   0b00001000,
   0b10001000,
   0b01110000,   //letter 5

   0b01110000,
   0b10000000,
   0b10000000,
   0b11110000,
   0b10001000,
   0b10001000,
   0b01110000,   //letter 6

   0b11111000,
   0b00001000,
   0b00001000,
   0b00010000,
   0b00100000,
   0b01000000,
   0b10000000,   //letter 7

   0b01110000,
   0b10001000,
   0b10001000,
   0b01110000,
   0b10001000,
   0b10001000,
   0b01110000,   //letter 8

   0b01110000,
   0b10001000,
   0b10001000,
   0b01111000,
   0b00010000,
   0b00100000,
   0b01000000,   //letter 9

   0b00000000,
   0b00000000,
   0b01100000,
   0b01100000,
   0b00000000,
   0b01100000,
   0b01100000,    //letter :

   0b00000000,
   0b00000000,
   0b01100000,
   0b01100000,
   0b00000000,
   0b00100000,
   0b01000000,    //letter ;

   0b00010000,
   0b00100000,
   0b01000000,
   0b10000000,
   0b01000000,
   0b00100000,
   0b00010000,    //letter <

   0b00000000,
   0b00000000,
   0b11111000,
   0b00000000,
   0b11111000,
   0b00000000,
   0b00000000,    //letter =

   0b01000000,
   0b00100000,
   0b00010000,
   0b00001000,
   0b00010000,
   0b00100000,
   0b01000000,    //letter >

   0b01110000,
   0b10001000,
   0b00001000,
   0b00110000,
   0b01000000,
   0b00000000,
   0b01000000,    //letter ?

   0b01110000,
   0b10001000,
   0b10111000,
   0b10101000,
   0b10111000,
   0b10000000,
   0b01111000,    //letter @

//-----------------------------------Capital Letters
   0b00100000,
   0b01010000,
   0b10001000,
   0b11111000,
   0b10001000,
   0b10001000,
   0b10001000,    //letter A

   0b11110000,
   0b10001000,
   0b10001000,
   0b11110000,
   0b10001000,
   0b10001000,
   0b11110000,    //letter B

   0b01110000,
   0b10001000,
   0b10000000,
   0b10000000,
   0b10000000,
   0b10001000,
   0b01110000,    //letter C

   0b11110000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b11110000,    //letter D

   0b11111000,
   0b10000000,
   0b10000000,
   0b11110000,
   0b10000000,
   0b10000000,
   0b11111000,    //letter E

   0b11111000,
   0b10000000,
   0b10000000,
   0b11110000,
   0b10000000,
   0b10000000,
   0b10000000,    //letter F

   0b01110000,
   0b10001000,
   0b10000000,
   0b10000000,
   0b10011000,
   0b10001000,
   0b01111000,    //letter G

   0b10001000,
   0b10001000,
   0b10001000,
   0b11111000,
   0b10001000,
   0b10001000,
   0b10001000,    //letter H

   0b01110000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b01110000,    //letter I

   0b11111000,
   0b00010000,
   0b00010000,
   0b00010000,
   0b10010000,
   0b10010000,
   0b01100000,    //letter J

   0b10001000,
   0b10010000,
   0b10100000,
   0b11000000,
   0b10100000,
   0b10010000,
   0b10001000,    //letter K

   0b10000000,
   0b10000000,
   0b10000000,
   0b10000000,
   0b10000000,
   0b10000000,
   0b11111000,    //letter L

   0b10001000,
   0b11011000,
   0b10101000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,    //letter M

   0b10001000,
   0b11001000,
   0b10101000,
   0b10011000,
   0b10001000,
   0b10001000,
   0b10001000,    //letter N

   0b01110000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b01110000,    //letter O

   0b11110000,
   0b10001000,
   0b10001000,
   0b11110000,
   0b10000000,
   0b10000000,
   0b10000000,    //letter P

   0b01110000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10101000,
   0b10011000,
   0b01111000,    //letter Q

   0b11110000,
   0b10001000,
   0b10001000,
   0b11110000,
   0b10100000,
   0b10010000,
   0b10001000,    //letter R

   0b01111000,
   0b10000000,
   0b10000000,
   0b01110000,
   0b00001000,
   0b00001000,
   0b11110000,    //letter S

   0b11111000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,    //letter T

   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b01110000,    //letter U

   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b01010000,
   0b00100000,    //letter V

   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10101000,
   0b11011000,
   0b10001000,    //letter W

   0b10001000,
   0b10001000,
   0b01010000,
   0b00100000,
   0b01010000,
   0b10001000,
   0b10001000,    //letter X

   0b10001000,
   0b10001000,
   0b10001000,
   0b01010000,
   0b00100000,
   0b00100000,
   0b00100000,    //letter Y

   0b11111000,
   0b00001000,
   0b00010000,
   0b00100000,
   0b01000000,
   0b10000000,
   0b11111000,    //letter Z
//-----------------------------------End capital letters
   0b11100000,
   0b10000000,
   0b10000000,
   0b10000000,
   0b10000000,
   0b10000000,
   0b11100000,    //letter [

   0b00000000,
   0b10000000,
   0b01000000,
   0b00100000,
   0b00010000,
   0b00001000,
   0b00000000,    //letter

   0b00111000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00111000,    //letter ]

   0b00100000,
   0b01010000,
   0b10001000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,    //letter ^

   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b11111000,    //letter _

   0b00010000,
   0b00010000,
   0b00001000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,    //letter '

   0b00000000,
   0b00000000,
   0b01110000,
   0b10001000,
   0b10001000,
   0b10011000,
   0b01101000,    //letter a

   0b10000000,
   0b10000000,
   0b11110000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b11110000,    //letter b

   0b00000000,
   0b00000000,
   0b01111000,
   0b10000000,
   0b10000000,
   0b10000000,
   0b01111000,    //letter c

   0b00001000,
   0b00001000,
   0b01111000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b01111000,    //letter d

   0b00000000,
   0b00000000,
   0b01110000,
   0b10001000,
   0b11111000,
   0b10000000,
   0b01111000,    //letter e

   0b00010000,
   0b00101000,
   0b01110000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,    //letter f

   0b00000000,
   0b00000000,
   0b01110000,
   0b10001000,
   0b01111000,
   0b00001000,
   0b01110000,    //letter g

   0b10000000,
   0b10000000,
   0b11110000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10001000,    //letter h

   0b00100000,
   0b00000000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,    //letter i

   0b00100000,
   0b00000000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b10100000,
   0b01000000,    //letter j

   0b10000000,
   0b10000000,
   0b10001000,
   0b10010000,
   0b10100000,
   0b11010000,
   0b10001000,    //letter k

   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00100000,    //letter l

   0b00000000,
   0b00000000,
   0b11010000,
   0b10101000,
   0b10101000,
   0b10101000,
   0b10101000,    //letter m

   0b00000000,
   0b00000000,
   0b10110000,
   0b11001000,
   0b10001000,
   0b10001000,
   0b10001000,    //letter n

   0b00000000,
   0b00000000,
   0b01110000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b01110000,    //letter o

   0b00000000,
   0b00000000,
   0b11110000,
   0b10001000,
   0b11110000,
   0b10000000,
   0b10000000,    //letter p

   0b00000000,
   0b00000000,
   0b01111000,
   0b10001000,
   0b01111000,
   0b00001000,
   0b00001000,    //letter q

   0b00000000,
   0b00000000,
   0b10110000,
   0b11001000,
   0b10000000,
   0b10000000,
   0b10000000,    //letter r

   0b00000000,
   0b00000000,
   0b01111000,
   0b10000000,
   0b01110000,
   0b00001000,
   0b11110000,    //letter s

   0b00100000,
   0b00100000,
   0b01110000,
   0b00100000,
   0b00100000,
   0b00100000,
   0b00110000,    //letter t

   0b00000000,
   0b00000000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b10011000,
   0b01101000,    //letter u

   0b00000000,
   0b00000000,
   0b10001000,
   0b10001000,
   0b10001000,
   0b01010000,
   0b00100000,    //letter v

   0b00000000,
   0b00000000,
   0b10001000,
   0b10001000,
   0b10101000,
   0b10101000,
   0b01010000,    //letter w

   0b00000000,
   0b00000000,
   0b10001000,
   0b01010000,
   0b00100000,
   0b01010000,
   0b10001000,    //letter x

   0b00000000,
   0b00000000,
   0b10001000,
   0b10001000,
   0b11111000,
   0b00001000,
   0b01110000,    //letter y

   0b00000000,
   0b00000000,
   0b11111000,
   0b00010000,
   0b00100000,
   0b01000000,
   0b11111000,    //letter z

   0b00100000,
   0b01000000,
   0b01000000,
   0b10000000,
   0b01000000,
   0b01000000,
   0b00100000,    //letter {

   0b00100000,
   0b00100000,
   0b00100000,
   0b00000000,
   0b00100000,
   0b00100000,
   0b00100000,    //letter |

   0b00100000,
   0b00010000,
   0b00010000,
   0b00001000,
   0b00010000,
   0b00010000,
   0b00100000,    //letter }

   0b01010000,
   0b10100000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000,
   0b00000000    //letter ~

};

//*********************************************************************
SDA5708::SDA5708(int loadpin, int datapin, int sdclkpin, int resetpin)
{
   _DATA  = datapin;
   _LOAD  = loadpin;
   _SDCLK = sdclkpin;
   _RESET = resetpin;

   // set the digital pin as output:
   pinMode(_DATA, OUTPUT);
   pinMode(_LOAD, OUTPUT);
   pinMode(_SDCLK, OUTPUT);
   pinMode(_RESET, OUTPUT);

   // toggle RESET
   digitalWrite(_LOAD, HIGH);
   digitalWrite(_RESET, LOW);
   digitalWrite(_RESET, HIGH);
}


//*********************************************************************
void SDA5708::send_byte_to_SDA5708(uint8_t byte)
{
	uint8_t x;

	// LOAD to Low
   digitalWrite(_LOAD, LOW);
	// send each digit to display
	for (x=0; x<=7;x++)
   {
		// set DATA
		if ((byte>>x)&1)
      {
         digitalWrite(_DATA, HIGH);
      }
      else
      {
         digitalWrite(_DATA, LOW);
      }
		// toggle SDCLK
      digitalWrite(_SDCLK, HIGH);
      digitalWrite(_SDCLK, LOW);
	}
	// set LOAD to High
   digitalWrite(_LOAD, HIGH);
}

//*********************************************************************
void SDA5708::digit_to_SDA5708(uint8_t sign, uint8_t digit)
{
	uint8_t i;
	// is it a valid sign? If no, we take a whitespace
	if ((sign < 0x20) || (sign > 0x7b)) sign = 0x20;
	// digit in range?
	if (digit > 7) digit = 0;
	// send digit address to display
	send_byte_to_SDA5708(0b10100000|digit);
	// address each line of the font, read it, put it out
	for(i=0; i<7; i++) {
		// font is left-justified, so move 3 bit to the right...
		send_byte_to_SDA5708(pgm_read_byte(&font[(sign-0x20)*7+i])/8);
	}
}

//*********************************************************************
void SDA5708::print2display(char *text, uint8_t cursor)
{
	char *p=text;
	while (*p)
  {
		digit_to_SDA5708(*p, cursor); //prints a character to the display at the given cursor_position 
		cursor++;
		p++;
	}
}

//*********************************************************************
void SDA5708::set_brightness(uint8_t val)
{
   // brightness from 0...8, 0 is maximum!
	// with the 4th bit one could manipulate the multiplex impulse
	// length  of the display controller to get it even darker (see
	// hardware-/protocol-description above.)
	send_byte_to_SDA5708(0b11100000|(val&0b00000111));
}
void SDA5708::clear() //clears the display
{
  for(int i = 0;i<16;i++)
  {
    digit_to_SDA5708(0x20,i);//print a whitespace to the display
  }
}
