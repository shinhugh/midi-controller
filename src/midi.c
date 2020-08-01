#include "common.h"
#include <avr/io.h>
#include "midi.h"

#include "serial_print.h" // DEBUG

#define MASK_NOTE 0x7fU
#define MASK_VELOCITY 0x7fU
#define STATUS_NOTE_OFF 0x80U
#define STATUS_NOTE_ON 0x90U
#define VELOCITY_NOTE_OFF 0x40U

// --------------------------------------------------

void midi_note_off(uint8_t note) {

  note &= MASK_NOTE;

  /*
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) (STATUS_NOTE_OFF);
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) (note);
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) (VELOCITY_NOTE_OFF);
  */

  // DEBUG START
  serial_print_hex(STATUS_NOTE_OFF);
  serial_print_letter(' ');
  serial_print_hex(note);
  serial_print_letter(' ');
  serial_print_hex(VELOCITY_NOTE_OFF);
  serial_print_newline();
  // DEBUG FINISH

}

// --------------------------------------------------

void midi_note_on(uint8_t note, uint8_t velocity) {

  note &= MASK_NOTE;
  velocity &= MASK_VELOCITY;

  /*
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) (STATUS_NOTE_ON);
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) (note);
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) (velocity);
  */

  // DEBUG START
  serial_print_hex(STATUS_NOTE_ON);
  serial_print_letter(' ');
  serial_print_hex(note);
  serial_print_letter(' ');
  serial_print_hex(velocity);
  serial_print_newline();
  // DEBUG FINISH

}
