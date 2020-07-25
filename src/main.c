// Program entry point

// CPU clock speed
#define F_CPU 16000000UL

#define DELAY_EN 1
#define DELAY_BUSY 100

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "common.h"
#include "display.h"


// --------------------------------------------------

void main() {

  /*
   * Liquid crystal LED 16x2 RGB display
   * - Register select: pin 12 (PB4)
   * - Enable: pin 11 (PB3)
   * - Data line d7: pin 7 (PD7)
   * - Data line d6: pin 8 (PB0)
   * - Data line d5: pin 9 (PB1)
   * - Data line d4: pin 10 (PB2)
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

  // ----------------------------------------

  // RGB: (255, 255, 255)
  PORTD |= (1 << PORTD6);
  PORTD |= (1 << PORTD5);
  PORTD |= (1 << PORTD3);

  // ----------------------------------------

  /*

  _delay_ms(200);

  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  PORTB |= (1 << PORTB2);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

  _delay_ms(5);

  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  PORTB |= (1 << PORTB2);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

  _delay_ms(1);

  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  PORTB |= (1 << PORTB2);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

  */

  _delay_ms(500);

  // Function set: 4-bit operation
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTB &= ~(1 << PORTB1);
  _delay_ms(DELAY_BUSY);

  // Function set: 4-bit operation, 2-line display, 5x8 character font
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTB &= ~(1 << PORTB1);
  _delay_ms(DELAY_BUSY);

  PORTB |= (1 << PORTB3);
  PORTD |= (1 << PORTD7);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTD &= ~(1 << PORTD7);
  _delay_ms(DELAY_BUSY);

  // Display control: display on, cursor off, blinking off
  PORTB |= (1 << PORTB3);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);

  PORTB |= (1 << PORTB3);
  PORTD |= (1 << PORTD7);
  PORTB |= (1 << PORTB0);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  _delay_ms(DELAY_BUSY);

  // Entry mode set: increment address, cursor shift right, no display shift
  PORTB |= (1 << PORTB3);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);

  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB0);
  PORTB |= (1 << PORTB1);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  _delay_ms(DELAY_BUSY);

  // ----------------------------------------

  // DDRAM address set
  PORTB |= (1 << PORTB3);
  PORTD |= (1 << PORTD7);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTD &= ~(1 << PORTD7);
  _delay_ms(DELAY_BUSY);

  PORTB |= (1 << PORTB3);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);

  // CGRAM/DDRAM write: 'H'
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB4);
  PORTB |= (1 << PORTB0);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTB &= ~(1 << PORTB4);
  PORTB &= ~(1 << PORTB0);
  _delay_ms(DELAY_BUSY);

  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB4);
  PORTD |= (1 << PORTD7);
  _delay_ms(DELAY_EN);
  PORTB &= ~(1 << PORTB3);
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  _delay_ms(DELAY_BUSY);

  // ----------------------------------------

  // Sleep
  sleep_enable();
  sei();
  sleep_cpu();
  sleep_disable();

  // ----------------------------------------

  // Loop
  while(1) {

    // TODO

  }

}
