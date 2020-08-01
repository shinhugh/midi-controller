// Program entry point

#include "common.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "midi.h"
#include "serial_print.h"
#include "display.h"

// --------------------------------------------------

// Pre-processor definitions

// Delay period at boot
#define DELAY_INIT 1000

// Clock ticks per timer1 overflow
#define TIMER1_LEN 16000

// Number of buttons
#define BUTTON_COUNT 64

// Number of bytes used to hold button states
#define BUTTON_STATE_BYTES 8

// Duration that new button state must hold to be acknowledged (ms)
#define BUTTON_ACK_DUR 1

// --------------------------------------------------

// Global variables

// Elapsed time in milliseconds (reset at 2^16)
volatile uint16_t elapsed_ms;

// Button input states, live (before debouncing)
volatile uint8_t button_state_pre[BUTTON_STATE_BYTES];

// Button input states, acknowledged (after debouncing)
volatile uint8_t button_state[BUTTON_STATE_BYTES];

// Data on buttons with unacknowledged states
// Bit[7]:   Unacknowledged state on/off
// Bit[6]:   End time wraps around due to overflow
// Bit[5:0]: Start time of unacknowledged state
volatile uint8_t button_unack_data[BUTTON_COUNT];

// --------------------------------------------------

// Interrupt handler for timer0 overflow
ISR(TIMER0_OVF_vect, ISR_NOBLOCK) {

  // Update buttons' live states
  if(PIND & (1 << PIND2)) {
    button_state_pre[0] &= ~(1 << 0);
  } else {
    button_state_pre[0] |= (1 << 0);
  }

  // TODO: Instead of internal timer interrupts, use interrupts from external
  // I/O expander, notified when external I/O ports change value

}

// --------------------------------------------------

// Interrupt handler for timer1 compare match
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK) {

  // Increment elapsed time
  elapsed_ms++;

}

// --------------------------------------------------

int main() {

  // Briefly pause before running any code to allow peripherals to boot
  _delay_ms(DELAY_INIT);

  // ----------------------------------------

  // Initialize global variables
  elapsed_ms = 0;
  for(uint8_t i = 0; i < BUTTON_STATE_BYTES; i++) {
    button_state_pre[i] = 0;
    button_state[i] = 0;
  }
  for(uint8_t i = 0; i < BUTTON_COUNT; i++) {
    button_unack_data[i];
  }

  // ----------------------------------------

  // Configure and enable timers
  TCNT0 = 0;
  TCCR0B = (1 << CS02);
  TIMSK0 = (1 << TOIE0);
  TCNT1 = 0;
  OCR1A = TIMER1_LEN - 1;
  TCCR1B = (1 << WGM12) | (1 << CS10);
  TIMSK1 = (1 << OCIE1A);

  // ----------------------------------------

  // Initialize USART
  UBRR0L = (uint8_t) 103; // 103 for 9600, 31 for 31250
  UCSR0B = (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

  // ----------------------------------------

  // Set pin directions

  // Button
  DDRD &= ~(1 << DDD2);

  // Built-in LED
  DDRB |= (1 << DDB5);

  // ----------------------------------------

  // Enable hardware interrupts
  sei();

  // ----------------------------------------

  // Stack variables

  uint8_t loop_count = 0;

  // DEBUG START
  display_init();
  display_clear();
  PORTD |= (1 << PORTD6);
  PORTD |= (1 << PORTD5);
  PORTD |= (1 << PORTD3);
  uint8_t press_count = 0;
  // DEBUG FINISH

  // ----------------------------------------

  // Loop until poweroff
  while(1) {

    // Infrequent code

    if(!(loop_count & 0x3f)) {
      // DEBUG START
      display_place_cursor(0, 0);
      display_write_number(button_state[0] & 0x01);
      display_place_cursor(0, 3);
      display_write_number(button_state_pre[0] & 0x01);
      display_place_cursor(1, 0);
      display_write_number(press_count);
      // DEBUG FINISH
    }

    // ----------------------------------------

    // Frequent code

    // Iterate through all buttons' live states and update acknowledged states
    for(uint8_t byte_index = 0; byte_index < BUTTON_STATE_BYTES; byte_index++) {
      for(uint8_t bit_index = 0; bit_index < 8; bit_index++) {

        // Information for current button
        uint8_t button_index = byte_index * 8 + bit_index;
        uint8_t curr_bit_pre
        = (button_state_pre[byte_index] >> bit_index) & 0x01;
        uint8_t curr_bit = (button_state[byte_index] >> bit_index) & 0x01;

        // If button's live state does not match acknowledged state
        if(curr_bit_pre != curr_bit) {

          // Sample current time (elapsed ms)
          uint8_t time_sample = ((uint8_t) elapsed_ms) & 0x3f;
          // Button unacknowledged state start time
          uint8_t start_time = button_unack_data[button_index] & 0x3f;

          // If unacknowledged state isn't set
          if(!button_unack_data[button_index]) {
            // Set unacknowledged state
            button_unack_data[button_index] = time_sample;
            if((button_unack_data[button_index] + BUTTON_ACK_DUR) >= 0x40) {
              button_unack_data[button_index] |= 0x40;
            }
            button_unack_data[button_index] |= 0x80;
          }

          // If unacknowledged state has held long enough
          else if(
            (
              !(button_unack_data[button_index] & 0x40)
              &&
              !(
                (start_time <= time_sample)
                &&
                (start_time + BUTTON_ACK_DUR > time_sample)
              )
            )
            ||
            (
              (button_unack_data[button_index] & 0x40)
              &&
              (
                (((start_time + BUTTON_ACK_DUR) & 0x3f) <= time_sample)
                &&
                (start_time > time_sample)
              )
            )
          ) {
            // Clear unacknowledged state
            button_unack_data[button_index] = 0;
            // Flip button state
            if(curr_bit) {
              button_state[byte_index] &= ~(1 << bit_index);
              uint8_t note = ((button_index / 6) * 13) + (button_index % 6);
              serial_print_number(press_count); // DEBUG
              serial_print_newline(); // DEBUG
              serial_print_string("Note Off: "); // DEBUG
              serial_print_number(note); // DEBUG
              serial_print_newline(); // DEBUG
              midi_note_off(note);
              serial_print_newline(); // DEBUG
              // TODO: Generate MIDI Note Off via serial for relevant note
            } else {
              button_state[byte_index] |= (1 << bit_index);
              uint8_t note = ((button_index / 6) * 13) + (button_index % 6);
              press_count++; // DEBUG
              serial_print_number(press_count); // DEBUG
              serial_print_newline(); // DEBUG
              serial_print_string("Note On: "); // DEBUG
              serial_print_number(note); // DEBUG
              serial_print_newline(); // DEBUG
              midi_note_on(note, 127);
              serial_print_newline(); // DEBUG
              // TODO: Generate MIDI Note On via serial for relevant note
            }
          }

        }

        // If button's live state matches acknowledged state
        else {
          // Clear unacknowledged state
          button_unack_data[button_index] = 0;
        }

      }
    }

    // ----------------------------------------

    loop_count++;

  }

  // ----------------------------------------

  return 0;

}
