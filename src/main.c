// Program entry point

#include "common.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "twi.h"
#include "midi.h"
#include "serial_print.h"
#include "display.h"

// --------------------------------------------------

// Pre-processor definitions

// Delay period at boot
#define DELAY_INIT 1000

// Clock ticks per timer1 overflow
#define TIMER1_LEN 16000

// Number of buttons (60, round up to multiple of 8)
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

// Whether a TWI operation is ongoing
volatile uint8_t twi_ongoing;

// --------------------------------------------------

/*
// Interrupt handler for timer0 overflow
ISR(TIMER0_OVF_vect, ISR_NOBLOCK) {

  // Update buttons' live states
  if(PIND & (1 << PIND2)) {
    button_state_pre[0] &= ~(1 << 0);
  } else {
    button_state_pre[0] |= (1 << 0);
  }

}
*/

// TODO: Instead of internal timer interrupts, use interrupts from external
//       I/O expander, notified when external I/O ports change value

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
  twi_ongoing = 0;

  // ----------------------------------------

  // Configure and enable timers
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

  // Initialize TWI (I2C)
  TWBR = 72;
  PORTC |= (1 << PORTC4);
  PORTC |= (1 << PORTC5);

  // ----------------------------------------

  // Set pin directions

  // Built-in LED
  DDRB |= (1 << DDB5);

  // Button bypass
  DDRD &= ~(1 << DDD2); // DEBUG
  // PORTD |= (1 << PORTD2); // DEBUG

  // ----------------------------------------

  // Enable hardware interrupts
  sei();

  // ----------------------------------------

  // Stack variables

  // Loop counter
  uint8_t loop_count = 0;

  // ----------------------------------------

  // One-time routine

  // DEBUG START
  display_init();
  display_clear();
  PORTD |= (1 << PORTD6);
  PORTD |= (1 << PORTD5);
  PORTD |= (1 << PORTD3);
  // DEBUG FINISH

  // Initialize MCP23017
  if(!twi_ongoing) {

    twi_ongoing = 1;

    // Transmit start condition
    if(twi_transmit_start()) {}
    // Transmit slave address + write
    if(twi_transmit_slaveaddr(0x20, 0)) {}
    // Transmit register address of IOCON
    if(twi_transmit_data(0x0a)) {}
    // Transmit IOCON value
    if(twi_transmit_data(0x00)) {}

    // Transmit restart condition
    if(twi_transmit_restart()) {}
    // Transmit slave address + write
    if(twi_transmit_slaveaddr(0x20, 0)) {}
    // Transmit register address of IODIRA
    if(twi_transmit_data(0x00)) {}
    // Transmit IODIRA value
    if(twi_transmit_data(0xff)) {}
    // Transmit IODIRB value
    if(twi_transmit_data(0xff)) {}

    // Transmit restart condition
    if(twi_transmit_restart()) {}
    // Transmit slave address + write
    if(twi_transmit_slaveaddr(0x20, 0)) {}
    // Transmit register address of GPPUA
    if(twi_transmit_data(0x0c)) {}
    // Transmit GPPUA value
    if(twi_transmit_data(0xff)) {}
    // Transmit GPPUB value
    if(twi_transmit_data(0xff)) {}

    // Transmit restart condition
    if(twi_transmit_restart()) {}
    // Transmit slave address + write
    if(twi_transmit_slaveaddr(0x20, 0)) {}
    // Transmit register address of IPOLA
    if(twi_transmit_data(0x02)) {}
    // Transmit IPOLA value
    if(twi_transmit_data(0x00)) {}
    // Transmit IPOLB value
    if(twi_transmit_data(0x00)) {}

    // Transmit stop condition
    twi_transmit_stop();

    twi_ongoing = 0;

  }

  // ----------------------------------------

  // Loop until poweroff
  while(1) {

    // Infrequent code

    // Once every 64 loop iterations
    if(!(loop_count & 0x3f)) {

    }

    // ----------------------------------------

    // Frequent code

    // Update all buttons' live (pre-debounce) states via TWI
    if(!twi_ongoing) {

      twi_ongoing = 1;

      // Transmit start condition
      if(twi_transmit_start()) {}
      // Transmit slave address + write
      if(twi_transmit_slaveaddr(0x20, 0)) {}
      // Transmit register address of GPIOA
      if(twi_transmit_data(0x12)) {}
      // Transmit restart condition
      if(twi_transmit_restart()) {}
      // Transmit slave address + read
      if(twi_transmit_slaveaddr(0x20, 1)) {}
      // Proceed to receive data byte, then transmit NACK
      if(twi_receive_data_ack(0)) {}

      // Update button live states accordingly
      button_state_pre[0] = ~TWDR;

      // Proceed to receive data byte, then transmit NACK
      if(twi_receive_data_nack(0)) {}

      // Update button live states accordingly
      button_state_pre[1] = ~TWDR;

      // Transmit stop condition
      twi_transmit_stop();

      twi_ongoing = 0;

    }

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
            // Flip button state and generate MIDI event
            if(curr_bit) {
              button_state[byte_index] &= ~(1 << bit_index);
              uint8_t note = ((button_index / 6) * 13) + (button_index % 6);
              midi_note_off(note);
            } else {
              button_state[byte_index] |= (1 << bit_index);
              uint8_t note = ((button_index / 6) * 13) + (button_index % 6);
              midi_note_on(note, 127);
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

    // Increment loop counter
    loop_count++;

  }

  // ----------------------------------------

  return 0;

}
