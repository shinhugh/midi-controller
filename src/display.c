#include "common.h"
#include <avr/io.h>
#include <util/delay.h>
#include "display.h"

// Delay after each instruction
#define DELAY_BUSY 2

// --------------------------------------------------

// RGB PWM values (0 ~ 100)
volatile unsigned int rgb_pwm_share_red, rgb_pwm_share_green,
rgb_pwm_share_blue;
volatile unsigned char rgb_incr_color;
volatile unsigned int rgb_incr_color_val;

// --------------------------------------------------

void display_init() {

  rgb_pwm_share_red = 0;
  rgb_pwm_share_green = 0;
  rgb_pwm_share_blue = 0;
  rgb_incr_color = 0;
  rgb_incr_color_val = PWM_DEPTH;

  // ----------------------------------------

  // Set pin 12 to output mode
  DDRB |= (1 << DDB4);
  // Set pin 11 to output mode
  DDRB |= (1 << DDB3);
  // Set pin 10 to output mode
  DDRB |= (1 << DDB2);
  // Set pin 9 to output mode
  DDRB |= (1 << DDB1);
  // Set pin 8 to output mode
  DDRB |= (1 << DDB0);
  // Set pin 7 to output mode
  DDRD |= (1 << DDD7);
  // Set pin 6 to output mode
  DDRD |= (1 << DDD6);
  // Set pin 5 to output mode
  DDRD |= (1 << DDD5);
  // Set pin 3 to output mode
  DDRD |= (1 << DDD3);

  // ----------------------------------------

  // Function set: 4-bit operation
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB &= ~(1 << PORTB1);

  // Function set: 4-bit operation, 2-line display, 5x8 character font
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB1);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB &= ~(1 << PORTB1);
  PORTB |= (1 << PORTB3);
  PORTD |= (1 << PORTD7);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTD &= ~(1 << PORTD7);

  // Display control: display on, cursor off, blinking off
  PORTB |= (1 << PORTB3);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB |= (1 << PORTB3);
  PORTD |= (1 << PORTD7);
  PORTB |= (1 << PORTB0);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);

  // Entry mode set: increment address, cursor shift right, no display shift
  PORTB |= (1 << PORTB3);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB0);
  PORTB |= (1 << PORTB1);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);

}

// --------------------------------------------------

void display_clear() {

  PORTB |= (1 << PORTB3);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB |= (1 << PORTB3);
  PORTB |= (1 << PORTB2);
  PORTB &= ~(1 << PORTB3);
  _delay_ms(DELAY_BUSY);
  PORTB &= ~(1 << PORTB2);

}

// --------------------------------------------------

void display_write_char(unsigned int code) {

  // 8-bit character codes
  code %= 256;

  // Enable: rising edge
  PORTB |= (1 << PORTB3);

  // Register select: data
  PORTB |= (1 << PORTB4);
  // d7
  if(0x80 & code) {
    PORTD |= (1 << PORTD7);
  } else {
    PORTD &= ~(1 << PORTD7);
  }
  // d6
  if(0x40 & code) {
    PORTB |= (1 << PORTB0);
  } else {
    PORTB &= ~(1 << PORTB0);
  }
  // d5
  if(0x20 & code) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }
  // d4
  if(0x10 & code) {
    PORTB |= (1 << PORTB2);
  } else {
    PORTB &= ~(1 << PORTB2);
  }

  // Enable: falling edge
  PORTB &= ~(1 << PORTB3);

  // Delay
  _delay_ms(DELAY_BUSY);

  // Reset registers
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

  // Enable: rising edge
  PORTB |= (1 << PORTB3);

  // Register select: data
  PORTB |= (1 << PORTB4);
  // d7
  if(0x08 & code) {
    PORTD |= (1 << PORTD7);
  } else {
    PORTD &= ~(1 << PORTD7);
  }
  // d6
  if(0x04 & code) {
    PORTB |= (1 << PORTB0);
  } else {
    PORTB &= ~(1 << PORTB0);
  }
  // d5
  if(0x02 & code) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }
  // d4
  if(0x01 & code) {
    PORTB |= (1 << PORTB2);
  } else {
    PORTB &= ~(1 << PORTB2);
  }

  // Enable: falling edge
  PORTB &= ~(1 << PORTB3);

  // Delay
  _delay_ms(DELAY_BUSY);

  // Reset registers
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

}

// --------------------------------------------------

void display_write_number(unsigned int number) {

  unsigned int i;

  unsigned int length = 0;
  unsigned int number_copy = number;
  while(number_copy > 0) {
    length++;
    number_copy /= 10;
  }
  length = length == 0 ? 1 : length;

  unsigned int curr_digit;
  unsigned int divisor = 1;
  for(i = 0; i < length - 1; i++) {
    divisor *= 10;
  }
  for(i = 0; i < length; i++) {
    curr_digit = (unsigned int) ((number / divisor) % 10);
    display_write_char(0x30 + curr_digit);
    divisor /= 10;
  }

}

// --------------------------------------------------

void display_place_cursor(unsigned int row, unsigned int col) {

  unsigned int index = (0x40 * row) + col;

  // Enable: rising edge
  PORTB |= (1 << PORTB3);

  // Register select: instruction
  PORTB &= ~(1 << PORTB4);
  // d7
  PORTD |= (1 << PORTD7);
  // d6
  if(0x40 & index) {
    PORTB |= (1 << PORTB0);
  } else {
    PORTB &= ~(1 << PORTB0);
  }
  // d5
  if(0x20 & index) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }
  // d4
  if(0x10 & index) {
    PORTB |= (1 << PORTB2);
  } else {
    PORTB &= ~(1 << PORTB2);
  }

  // Enable: falling edge
  PORTB &= ~(1 << PORTB3);

  // Delay
  _delay_ms(DELAY_BUSY);

  // Reset registers
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

  // Enable: rising edge
  PORTB |= (1 << PORTB3);

  // Register select: instruction
  PORTB &= ~(1 << PORTB4);
  // d7
  if(0x08 & index) {
    PORTD |= (1 << PORTD7);
  } else {
    PORTD &= ~(1 << PORTD7);
  }
  // d6
  if(0x04 & index) {
    PORTB |= (1 << PORTB0);
  } else {
    PORTB &= ~(1 << PORTB0);
  }
  // d5
  if(0x02 & index) {
    PORTB |= (1 << PORTB1);
  } else {
    PORTB &= ~(1 << PORTB1);
  }
  // d4
  if(0x01 & index) {
    PORTB |= (1 << PORTB2);
  } else {
    PORTB &= ~(1 << PORTB2);
  }

  // Enable: falling edge
  PORTB &= ~(1 << PORTB3);

  // Delay
  _delay_ms(DELAY_BUSY);

  // Reset registers
  PORTB &= ~(1 << PORTB4);
  PORTD &= ~(1 << PORTD7);
  PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB1);
  PORTB &= ~(1 << PORTB2);

}

// --------------------------------------------------

void display_pwm(unsigned int pwm_curr) {

  // Update backlight RGB
  if(pwm_curr) {
    if(pwm_curr == rgb_pwm_share_red) {
      PORTD &= ~(1 << PORTD6);
    }
    if(pwm_curr == rgb_pwm_share_green) {
      PORTD &= ~(1 << PORTD5);
    }
    if(pwm_curr == rgb_pwm_share_blue) {
      PORTD &= ~(1 << PORTD3);
    }
  } else {
    if(rgb_pwm_share_red) {
      PORTD |= (1 << PORTD6);
    }
    else {
      PORTD &= ~(1 << PORTD6);
    }
    if(rgb_pwm_share_green) {
      PORTD |= (1 << PORTD5);
    }
    else {
      PORTD &= ~(1 << PORTD5);
    }
    if(rgb_pwm_share_blue) {
      PORTD |= (1 << PORTD3);
    }
    else {
      PORTD &= ~(1 << PORTD3);
    }
  }

}

// --------------------------------------------------

void display_set_backlight_rgb(unsigned int rgb_red, unsigned int rgb_green,
unsigned int rgb_blue) {

  rgb_pwm_share_red = rgb_red > PWM_DEPTH ? PWM_DEPTH : rgb_red;
  rgb_pwm_share_green = rgb_green > PWM_DEPTH ? PWM_DEPTH : rgb_green;
  rgb_pwm_share_blue = rgb_blue > PWM_DEPTH ? PWM_DEPTH : rgb_blue;

}

// --------------------------------------------------

void display_backlight_rgb_trans() {

  if(rgb_incr_color == 0) {
    display_set_backlight_rgb(rgb_incr_color_val, 0,
    PWM_DEPTH - rgb_incr_color_val);
    if(rgb_incr_color_val == PWM_DEPTH) {
      rgb_incr_color = 1;
      rgb_incr_color_val = 1;
    } else {
      rgb_incr_color_val++;
    }
  }
  if(rgb_incr_color == 1) {
    display_set_backlight_rgb(PWM_DEPTH - rgb_incr_color_val,
    rgb_incr_color_val, 0);
    if(rgb_incr_color_val == PWM_DEPTH) {
      rgb_incr_color = 2;
      rgb_incr_color_val = 1;
    } else {
      rgb_incr_color_val++;
    }
  }
  if(rgb_incr_color == 2) {
    display_set_backlight_rgb(0, PWM_DEPTH - rgb_incr_color_val,
    rgb_incr_color_val);
    if(rgb_incr_color_val == PWM_DEPTH) {
      rgb_incr_color = 0;
      rgb_incr_color_val = 1;
    } else {
      rgb_incr_color_val++;
    }
  }

}
