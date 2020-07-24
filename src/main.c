// Program entry point

#include "common.h"
#include <avr/io.h>
#include <util/delay.h>


// --------------------------------------------------

void main() {

  /*
   * Liquid crystal LED 16x2 RGB display
   * - Register select: pin 12 (PB4)
   * - Enable: pin 11 (PB3)
   * - Data line d4: pin 10 (PB2)
   * - Data line d5: pin 9 (PB1)
   * - Data line d6: pin 8 (PB0)
   * - Data line d7: pin 7 (PD7)
   * - Backlight RGB red: pin 6 (PD6)
   * - Backlight RGB green: pin 5 (PD5)
   * - Backlight RGB blue: pin 3 (PD3)
   */

   // Set pin 12 to output mode
   // Set bit 4 of DDRB to 1
   DDRB |= (1 << DDB4);
   // Set pin 11 to output mode
   // Set bit 3 of DDRB to 1
   DDRB |= (1 << DDB3);
   // Set pin 10 to output mode
   // Set bit 2 of DDRB to 1
   DDRB |= (1 << DDB2);
   // Set pin 9 to output mode
   // Set bit 1 of DDRB to 1
   DDRB |= (1 << DDB1);
   // Set pin 8 to output mode
   // Set bit 0 of DDRB to 1
   DDRB |= (1 << DDB0);
   // Set pin 7 to output mode
   // Set bit 7 of DDRD to 1
   DDRD |= (1 << DDD7);
   // Set pin 6 to output mode
   // Set bit 6 of DDRD to 1
   DDRD |= (1 << DDD6);
   // Set pin 5 to output mode
   // Set bit 5 of DDRD to 1
   DDRD |= (1 << DDD5);
   // Set pin 3 to output mode
   // Set bit 3 of DDRD to 1
   DDRD |= (1 << DDD3);

   // RGB: (255, 255, 255)
   PORTD |= (1 << PORTD6);
   PORTD |= (1 << PORTD5);
   PORTD |= (1 << PORTD3);

  // Loop
  while(1) {

    // TODO

  }

}
