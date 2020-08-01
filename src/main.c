// Program entry point

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "midi.h"
#include "serial_print.h"
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

// Bitmask with least significant 8 bits
#define MASK_LSB 0xffU

// Delay period at the very beginning of program
#define DELAY_INIT 1000U

// Bitmask to apply on timer0 overflow counter to limit handler frequency
#define MASK_TIMER0_OVF_COUNT 0x0fU

// Clock ticks per timer1 overflow
#define TIMER1_LEN 16000U

// Number of buttons
#define BUTTON_CONNECTIONS 1

// Duration that new button press state must hold to be acknowledged
#define BUTTON_HOLD_ACK 32U

// --------------------------------------------------

// Lock on timer0 overflow handler
volatile uint8_t lock_timer0_ovf;

// Number of timer0 overflows
volatile uint8_t count_timer0_ovf;

// Elapsed time in milliseconds
volatile uint64_t elapsed_ms;

// Button press state
volatile uint8_t button_press_state[BUTTON_CONNECTIONS];
// Duration that a different button state has held for
volatile uint8_t button_hold_curr[BUTTON_CONNECTIONS];
// Number of acknowledged button presses thus far (serves as a global queue)
volatile uint8_t button_press_count[BUTTON_CONNECTIONS];
// Number of acknowledged button releases thus far (serves as a global queue)
volatile uint8_t button_release_count[BUTTON_CONNECTIONS];

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

    // Decrease frequency of handler code execution
    if(!(count_timer0_ovf & MASK_TIMER0_OVF_COUNT)) {

      for(uint16_t button_index = 0; button_index < BUTTON_CONNECTIONS;
      button_index++) {
        // Update button press state
        if(!(PIND & (1 << PIND2)) ^ button_press_state[button_index]) {
          if(button_hold_curr[button_index] >= BUTTON_HOLD_ACK) {
            button_press_state[button_index]
            = !button_press_state[button_index];
            button_hold_curr[button_index] = 0;
            if(button_press_state[button_index]) {
              button_press_count[button_index]++;
            } else {
              button_release_count[button_index]++;
            }
          } else {
            button_hold_curr[button_index]++;
          }
        } else {
          button_hold_curr[button_index] = 0;
        }
      }

      /*
      // Transition display backlight RGB
      time_sample_curr_rgb = elapsed_ms;
      if(time_sample_curr_rgb != time_sample_last_rgb) {
        display_backlight_rgb_trans();
      }
      time_sample_last_rgb = time_sample_curr_rgb;
      */

    }

    // Unlock interrupt handler
    lock_timer0_ovf = 0;

  }

  // Increment overflow counter
  count_timer0_ovf++;

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
  count_timer0_ovf = 0;
  elapsed_ms = 0;
  time_sample_curr_rgb = 0;
  time_sample_last_rgb = 0;
  for(uint16_t button_index = 0; button_index < BUTTON_CONNECTIONS;
  button_index++) {
    button_press_state[button_index] = 0;
    button_hold_curr[button_index] = 0;
    button_press_count[button_index] = 0;
    button_release_count[button_index] = 0;
  }

  // ----------------------------------------

  // Configure and enable timers
  TCNT0 = 0;
  TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B = (1 << CS00);
  TIMSK0 = (1 << TOIE0);
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

  // Initialize USART
  // Baud: 103 for 9600, 31 for 31250
  UBRR0L = (uint8_t) 103;
  UCSR0B = (1 << TXEN0); // | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

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
  // display_backlight_rgb_trans_on();
  display_backlight_rgb_trans_off();
  display_set_backlight_rgb(PWM_MAX, PWM_MAX, PWM_MAX);
  display_clear();

  // ----------------------------------------

  // Stack variables
  uint8_t button_on_count[BUTTON_CONNECTIONS];
  uint8_t button_off_count[BUTTON_CONNECTIONS];
  for(uint16_t button_index = 0; button_index < BUTTON_CONNECTIONS;
  button_index++) {
    button_on_count[button_index] = 0;
    button_off_count[button_index] = 0;
  }

  // ----------------------------------------

  // Main program loop
  while(1) {

    // Print elapsed seconds
    // display_place_cursor(0, 0);
    // display_write_number(elapsed_ms / 1000U);

    // Button-triggered MIDI
    while(1) {
      uint8_t break_condition = 1;
      for(uint16_t button_index = 0; button_index < BUTTON_CONNECTIONS;
      button_index++) {
        if(button_on_count[button_index] != button_press_count[button_index]) {
          button_on_count[button_index]++;
          break_condition = 0;
          midi_note_on(0, 127);
        }
        if(button_off_count[button_index]
        != button_release_count[button_index]) {
          button_off_count[button_index]++;
          break_condition = 0;
          midi_note_off(0);
        }
      }
      if(break_condition) {
        break;
      }
    }

    // Display button press/release counts on LED display
    display_place_cursor(1, 0);
    display_write_number(button_on_count[0]);
    display_place_cursor(1, 8);
    display_write_number(button_off_count[0]);

  }

  // ----------------------------------------

  return 0;

}
