// Program entry point

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "display.h"

#define OVF_FREQ_RGB_PWM 12
#define OVF_FREQ_RGB_TRANS 1600

// --------------------------------------------------

volatile unsigned int pwm_curr;
volatile unsigned int ovf_count_rgb_pwm;
volatile unsigned int ovf_count_rgb_trans;
volatile unsigned char rgb_incr_color;
volatile unsigned char rgb_incr_color_val;

// --------------------------------------------------

ISR(TIMER0_OVF_vect, ISR_BLOCK) {

  if(ovf_count_rgb_pwm >= OVF_FREQ_RGB_PWM) {
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
    pwm_curr = (pwm_curr + 1) % PWM_DEPTH;
    ovf_count_rgb_pwm = 0;
  }
  ovf_count_rgb_pwm++;

  if(ovf_count_rgb_trans >= OVF_FREQ_RGB_TRANS) {
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
    ovf_count_rgb_trans = 0;
  }
  ovf_count_rgb_trans++;

}

// --------------------------------------------------

void main() {

  _delay_ms(DELAY_INIT);

  // ----------------------------------------

  pwm_curr = 0;
  ovf_count_rgb_pwm = 0;
  ovf_count_rgb_trans = 0;
  rgb_incr_color = 0;
  rgb_incr_color_val = PWM_DEPTH;

  // ----------------------------------------

  // Set pin 2 to input mode
  DDRD &= ~(1 << DDD2);

  // ----------------------------------------

  display_init();

  // ----------------------------------------

  TCNT0 = 0;
  TCCR0B = (1 << CS00);
  TIMSK0 = (1 << TOIE0);
  sei();

  // ----------------------------------------

  display_clear();
  display_write_char(0x48); // 'H'
  display_write_char(0x65); // 'e'
  display_write_char(0x6c); // 'l'
  display_write_char(0x6c); // 'l'
  display_write_char(0x6f); // 'o'
  display_write_char(0x2c); // comma
  display_write_char(0x20); // space
  display_write_char(0xcb); // 'hi' [katakana]
  display_write_char(0xad); // 'yu' (small) [katakana]
  display_write_char(0xb0); // '-' [katakana]

  // ----------------------------------------

  // Loop
  while(1);

}
