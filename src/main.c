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
 *
 * Arcade button
 * - Press: PD2
 */

// --------------------------------------------------

// Delay period at the very beginning of program
#define DELAY_INIT 100

// Width of PWM slot, given in log of number of timer overflows
#define OVF_WIDTH_PWM 0

// --------------------------------------------------

// Flag set when timer0 overflow handler is running, to avoid stack overflow
volatile uint8_t timer0_ovf_handler_process;

// Timer1 overflow counter
volatile uint32_t timer1_ovf_count;

// Quantization value for elapsed time
volatile uint8_t snap_curr_elapsed_s, snap_last_elapsed_s;
// Elapsed time in seconds
volatile uint64_t elapsed_s;

// Quantization value for PWM
volatile uint8_t snap_curr_pwm, snap_last_pwm;
// PWM index
volatile uint8_t pwm;

// DEBUG START
volatile uint32_t pwm_cycle_count;
volatile uint8_t timer0_ovf_handled_count;
// DEBUG FINISH

// --------------------------------------------------

// Interrupt handler for timer0 overflow
ISR(TIMER0_OVF_vect, ISR_NOBLOCK) {

  if(!timer0_ovf_handler_process) {

    // Prevent nested interrupts for this handler
    timer0_ovf_handler_process = 1;

    // ----------------------------------------

    // Update PWM slot index
    snap_curr_pwm = timer1_ovf_count; // >> OVF_WIDTH_PWM
    if(snap_curr_pwm != snap_last_pwm) {
      pwm++;
      snap_last_pwm = snap_curr_pwm;

      // DEBUG START
      if(!pwm) {
        pwm_cycle_count++;
      }
      // DEBUG FINISH
    }

    // ----------------------------------------

    // Update elapsed seconds
    snap_curr_elapsed_s = (timer1_ovf_count << 16) / F_CPU;
    if(snap_curr_elapsed_s != snap_last_elapsed_s) {
      elapsed_s++;
      snap_last_elapsed_s = snap_curr_elapsed_s;
    }

    // ----------------------------------------

    // Update PWM output for display backlight RGB
//    display_pwm(pwm);

    // ----------------------------------------

    // DEBUG START
    if((timer0_ovf_handled_count >> 7) & 0x01) {
      PORTB |= (1 << PORTB5);
    } else {
      PORTB &= ~(1 << PORTB5);
    }
    timer0_ovf_handled_count++;
    // DEBUG FINISH

    // Release lock on future instances of this handler
    timer0_ovf_handler_process = 0;

  }

}

// --------------------------------------------------

// Interrupt handler for timer1 overflow
ISR(TIMER1_OVF_vect, ISR_BLOCK) {

  // Increment timer overflow counter
  timer1_ovf_count++;

}

// --------------------------------------------------

// Program entry point
int main() {

  // Briefly pause before running any code to allow peripherals to boot
  _delay_ms(DELAY_INIT);

  // ----------------------------------------

  // Initialize global variables
  timer0_ovf_handler_process = 0;
  timer1_ovf_count = 0;
  snap_curr_elapsed_s = 0;
  snap_last_elapsed_s = 0;
  elapsed_s = 0;
  snap_curr_pwm = 0;
  snap_last_pwm = 0;
  pwm = 0;

  // DEBUG START
  pwm_cycle_count = 0;
  timer0_ovf_handled_count = 0;
  // DEBUG FINISH

  // ----------------------------------------

  // Set pin directions
  DDRD &= ~(1 << DDD2);
  DDRB |= (1 << DDB5);

  // ----------------------------------------

  // Configure and enable timers
  TCNT0 = 0;
  TCCR0B = (1 << CS02);
  TIMSK0 = (1 << TOIE0);
  TCNT1 = 0;
  TCCR1B = (1 << CS10);
  TIMSK1 = (1 << TOIE1);

  // ----------------------------------------

  // Initialize display
  display_init();

  // ----------------------------------------

  // Enable hardware interrupts
  sei();

  // ----------------------------------------

  // Configure display
//  display_set_backlight_rgb(10, 10, 10);
  display_clear();

  // ----------------------------------------

  // DEBUG START
  PORTD |= (1 << PORTD6);
  PORTD |= (1 << PORTD5);
  PORTD |= (1 << PORTD3);
  // DEBUG FINISH

  // Stack variables
  // TODO

  // DEBUG START
  display_place_cursor(0, 0);
  display_write_number(elapsed_s);
  display_place_cursor(1, 0);
  display_write_number(pwm_cycle_count);
  // DEBUG FINISH

  // ----------------------------------------

  // Main program loop
  while(1) {

    // Print elapsed seconds
    display_place_cursor(0, 0);
    display_write_number(elapsed_s);

    // DEBUG START
    display_place_cursor(1, 0);
    display_write_number(pwm_cycle_count);
    // DEBUG FINISH

  }

  // ----------------------------------------

  return 0;

}
