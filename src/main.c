// Program entry point

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
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
 * - Backlight RGB red: PD6, OC0A
 * - Backlight RGB green: PD5, OC0B
 * - Backlight RGB blue: PD3, OC2B
 *
 * Arcade button
 * - Press: PD2
 */

// --------------------------------------------------

// Delay period at the very beginning of program
#define DELAY_INIT 100

// Clock ticks per timer1 overflow
#define TIMER1_LEN 16000U

// --------------------------------------------------

// Lock on timer0 overflow handler
volatile uint8_t lock_timer0_ovf;

// Elapsed time in milliseconds
volatile uint64_t elapsed_ms;

// Samples of elapsed time
volatile uint8_t time_sample_curr_rgb;
volatile uint8_t time_sample_last_rgb;

// --------------------------------------------------

// Interrupt handler for timer0 overflow
ISR(TIMER0_OVF_vect, ISR_NOBLOCK) {

  // Check lock on interrupt handler
  if(!lock_timer0_ovf) {

    // Lock interrupt handler
    lock_timer0_ovf = 1;

    // Transition display backlight RGB
    time_sample_curr_rgb = elapsed_ms;
    if(time_sample_curr_rgb != time_sample_last_rgb) {
      display_backlight_rgb_trans();
    }
    time_sample_last_rgb = time_sample_curr_rgb;

    // Unlock interrupt handler
    lock_timer0_ovf = 0;

  }

}

// --------------------------------------------------

// Interrupt handler for timer1 compare match
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK) {

  // Increment elapsed time
  elapsed_ms++;

}

// --------------------------------------------------

// Program entry point
int main() {

  // Briefly pause before running any code to allow peripherals to boot
  _delay_ms(DELAY_INIT);

  // ----------------------------------------

  // Initialize global variables
  lock_timer0_ovf = 0;
  elapsed_ms = 0;
  time_sample_curr_rgb = 0;
  time_sample_last_rgb = 0;

  // ----------------------------------------

  // Configure and enable timers
  TCNT0 = 0;
  TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B = (1 << CS00);
  OCR0A = 0;
  OCR0B = 0;
  TCNT1 = 0;
  OCR1A = TIMER1_LEN - 1;
  TCCR1B = (1 << WGM12) | (1 << CS10);
  TIMSK1 = (1 << OCIE1A);
  TCNT2 = 0;
  TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS20);
  OCR2B = 0;

  // ----------------------------------------

  // Set pin directions
  DDRD &= ~(1 << DDD2);
  DDRB |= (1 << DDB5);

  // ----------------------------------------

  // Initialize display
  display_init();

  // ----------------------------------------

  // Enable hardware interrupts
  sei();

  // ----------------------------------------

  // Configure display
  display_set_backlight_rgb(PWM_MAX, PWM_MAX, PWM_MAX);
  display_clear();

  // ----------------------------------------

  // Stack variables
  // TODO

  // ----------------------------------------

  // Main program loop
  while(1) {

    // Print elapsed seconds
    display_place_cursor(0, 0);
    display_write_number(elapsed_ms / 1000);
    // display_write_char(0x48);
    // display_write_char(0x69);

  }

  // ----------------------------------------

  return 0;

}
