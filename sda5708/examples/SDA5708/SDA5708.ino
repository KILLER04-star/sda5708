#include <SDA5708.h>

// create instance of display -> calls constructor which does the init, set pins as parameters
SDA5708 sda5708(/* LOAD= */ 0 , /* DATA= */ 1, /* SDCLK= */ 2, /* RESET= */ 3);

void setup()
{
}

void loop() 
{ 
   // set brightness 
   // 0 == max, 7 == off
   sda5708.set_brightness(3);

   // output text
   sda5708.print2display("Hi World\0",0);
        
   while (1);
}
