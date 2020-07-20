// Program entry point

#include "common.h"
#include <avr/io.h>
#include <util/delay.h>


// --------------------------------------------------

void main() {

  // Set digital pin 13 to output mode
  // Set the 5th bit of DDRB to 1
  DDRB |= (1 << DDB5);

  // Set digital pin 2 to input mode
  // Set the 2nd bit of DDRD to 0
  DDRD &= ~(1 << DDD2);

  // Loop
  while(1) {

    // If digital pin 2 reads a high voltage, turn on LED
    if((PIND >> PIND2) & 1) {
      // Set pin 13 to HIGH
      // Set the 5th bit of PORTB to 1
      PORTB |= (1 << PORTB5);
    }
    // If digital pin 2 reads a low voltage, turn off LED
    else {
      // Set pin 13 to LOW
      // Set the 5th bit of PORTB to 0
      PORTB &= ~(1 << PORTB5);
    }

  }

}
