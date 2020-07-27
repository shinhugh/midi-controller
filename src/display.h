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

// --------------------------------------------------

extern volatile unsigned int rgb_pwm_share_red, rgb_pwm_share_green,
rgb_pwm_share_blue;

// --------------------------------------------------

void display_init();

void display_clear();

void display_write_char(unsigned int);

void display_write_number(unsigned int);

void display_place_cursor(unsigned int, unsigned int);

void display_pwm(unsigned int);

void display_set_backlight_rgb(unsigned int, unsigned int, unsigned int);

void display_backlight_rgb_trans();

// --------------------------------------------------

#endif
