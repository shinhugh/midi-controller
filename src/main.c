// Program entry point

#include "common.h"
#include <avr/io.h>
#include <util/delay.h>


// --------------------------------------------------

void main() {

  // Set digital pin 13 to output mode
  // Set the 5th bit of DDRB to 1
  DDRB |= (1 << DDB5);

  // Loop
  while(1) {

    // Set pin 13 to HIGH
    // Set the 5th bit of PORTB to 1
    PORTB |= (1 << PORTB5);

    // Pause
    _delay_ms(MS_DELAY);

    // Set pin 13 to LOW
    // Set the 5th bit of PORTB to 0
    PORTB &= ~(1 << PORTB5);

    // Pause
    _delay_ms(MS_DELAY);

  }

}
