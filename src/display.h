// Interface to display

#ifndef DISPLAY_H
#define DISPLAY_H

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

#include <stdint.h>

// --------------------------------------------------

void display_init();

void display_clear();

void display_write_char(uint8_t);

void display_write_number(uint64_t);

void display_place_cursor(uint16_t, uint16_t);

void display_set_backlight_rgb(uint8_t, uint8_t, uint8_t);

void display_backlight_rgb_trans();

// --------------------------------------------------

#endif
