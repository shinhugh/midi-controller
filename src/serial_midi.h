// MIDI communication via serial

#ifndef SERIAL_MIDI_H
#define SERIAL_MIDI_H

#include <stdint.h>

// --------------------------------------------------

void serial_midi_note_off(uint8_t);

void serial_midi_note_on(uint8_t, uint8_t);

// --------------------------------------------------

#endif
