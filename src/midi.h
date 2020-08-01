// MIDI communication via serial

#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

// --------------------------------------------------

void midi_note_off(uint8_t);

void midi_note_on(uint8_t, uint8_t);

// --------------------------------------------------

#endif
