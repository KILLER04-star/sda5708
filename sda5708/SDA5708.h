/**************************************************************
 *      Library for Display SDA5707
 *      ===========================
 *           Uwe Berger; 2009
 *
 * 20131101, randy:
 *  - rewrite for arduino IDE (http://blog.smue.org/archives/433-DBOX1-SDA5708-display-+-arduino.html)
 * 20150710, Felix Wiedemann:
 *  - made Arduino Module, translated to english
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
#ifndef SDA5708_h
#define SDA5708_h

#include "Arduino.h"

class SDA5708
{
  public:
    SDA5708(int loadpin, int datapin, int sdclkpin, int resetpin);
    void print2display(char *text, uint8_t cursor);
    void set_brightness(uint8_t val);
    void clear();

  private:
    int _LOAD  = 0;
    int _DATA  = 1;
    int _SDCLK  = 2;
    int _RESET  = 3;
    void send_byte_to_SDA5708(uint8_t byte);
    void brightness_SDA5708(uint8_t val);
    void digit_to_SDA5708(uint8_t sign, uint8_t digit);

};

#endif /* SDA5708_h */
