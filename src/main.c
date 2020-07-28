// Program entry point

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdint.h>
#include "display.h"

// --------------------------------------------------

/*
 * ATmega328P microcontroller
 * - 16 MHz clock
 *
 * Liquid crystal LED 16x2 RGB display
 * - Register select: PB4
 * - Enable: PB3
 * - Data line d7: PD7
 * - Data line d6: PB0
 * - Data line d5: PB1
 * - Data line d4: PB2
 * - Backlight RGB red: PD6
 * - Backlight RGB green: PD5
 * - Backlight RGB blue: PD3
 */

// --------------------------------------------------

// Value that timer overflow counter resets to 0 at
#define OVF_CNT_RST 4294967296

// Delay period at the very beginning of program
#define DELAY_INIT 100

// --------------------------------------------------

// Timer overflow counter
volatile uint32_t timer_ovf_count;

// --------------------------------------------------

// Interrupt handler for timer0 overflow
ISR(TIMER0_OVF_vect, ISR_BLOCK) {

  // Increment timer overflow counter
  timer_ovf_count = (timer_ovf_count + 1) % OVF_CNT_RST;

}

// --------------------------------------------------

// Program entry point
int main() {

  // Briefly pause before running any code to allow peripherals to boot
  _delay_ms(DELAY_INIT);

  // ----------------------------------------

  // Initialize global variables
  timer_ovf_count = 0;

  // ----------------------------------------

  // Set pin directions
  DDRD &= ~(1 << DDD2);
  DDRB |= (1 << DDB5);

  // ----------------------------------------

  // Configure and enable timer0
  TCNT0 = 0;
  TCCR0B = (1 << CS00);
  TIMSK0 = (1 << TOIE0);

  // ----------------------------------------

  // Initialize display
  display_init();

  // ----------------------------------------

  // Enable hardware interrupts
  sei();

  // ----------------------------------------

  // Configure display
  display_set_backlight_rgb(100, 100, 100);

  // ----------------------------------------

  // Initialize stack variables
  uint32_t counter = 0;

  // ----------------------------------------

  // Main program loop
  while(1) {

    display_write_number(counter);
    _delay_ms(1000);
    counter++;

  }

  // ----------------------------------------

  return 0;

}
