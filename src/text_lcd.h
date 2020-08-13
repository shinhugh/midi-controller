// Interface to text LCD

#ifndef TEXT_LCD_H
#define TEXT_LCD_H

/*
 * 16x2 text LCD, RGB backlight - HD44780 controller
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

void text_lcd_init();

void text_lcd_clear();

void text_lcd_write_char(uint8_t);

void text_lcd_write_number(uint64_t);

void text_lcd_place_cursor(uint16_t, uint16_t);

void text_lcd_set_backlight_rgb(uint8_t, uint8_t, uint8_t);

void text_lcd_backlight_rgb_trans();

void text_lcd_backlight_rgb_trans_on();

void text_lcd_backlight_rgb_trans_off();

// --------------------------------------------------

#endif
