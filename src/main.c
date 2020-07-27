// Program entry point

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "display.h"

// --------------------------------------------------

// Delay amount at boot
#define DELAY_INIT 100

// Length of a single PWM time unit, given by number of timer overflows
#define OVF_PERIOD_PWM 5

// Period between backlight RGB shift, given by number of timer overflows
#define OVF_PERIOD_RGB_TRANS 1000

// Period between button samples, given by number of timer overflows
#define OVF_PERIOD_BUTTON 20

// Number of consecutive button samples required to read pressed to be
// acknowledged
#define BUTTON_HOLD_ACK 20

// --------------------------------------------------

volatile unsigned int pwm_curr;
volatile unsigned int ovf_count_pwm;
volatile unsigned int ovf_count_rgb_trans;
volatile unsigned int ovf_count_button;
volatile unsigned int button_hold_curr;
volatile unsigned char button_state;

// --------------------------------------------------

ISR(TIMER0_OVF_vect, ISR_BLOCK) {

  // PWM
  if(ovf_count_pwm >= OVF_PERIOD_PWM) {
    // PWM updates
    display_pwm(pwm_curr);
    // Move PWM marker forward
    pwm_curr = (pwm_curr + 1) % PWM_DEPTH;
    // Reset timer overflow count for PWM
    ovf_count_pwm = 0;
  } else {
    ovf_count_pwm++;
  }

  // Display backlight RGB transition
  if(ovf_count_rgb_trans >= OVF_PERIOD_RGB_TRANS) {
    display_backlight_rgb_trans();
    ovf_count_rgb_trans = 0;
  } else {
    ovf_count_rgb_trans++;
  }

  // Button press
  if(ovf_count_button >= OVF_PERIOD_BUTTON) {
    if(!(PIND & (1 << PIND2)) ^ button_state) {
      if(button_hold_curr >= BUTTON_HOLD_ACK) {
        button_state = !button_state;
        button_hold_curr = 0;
      } else {
        button_hold_curr++;
      }
    } else {
      button_hold_curr = 0;
    }
    ovf_count_button = 0;
  } else {
    ovf_count_button++;
  }

}

// --------------------------------------------------

int main() {

  _delay_ms(DELAY_INIT);

  // ----------------------------------------

  // Initialize global variables
  pwm_curr = 0;
  ovf_count_pwm = 0;
  ovf_count_rgb_trans = 0;
  ovf_count_button = 0;
  button_hold_curr = 0;
  button_state = 0;

  // ----------------------------------------

  // Set pin 2 to input mode
  DDRD &= ~(1 << DDD2);
  // Set pin 13 to output mode
  DDRB |= (1 << DDB5);
  // Set SDA pin to output mode
  DDRC |= (1 << DDC4);
  // Set SCL pin to output mode
  DDRC |= (1 << DDC5);

  // ----------------------------------------

  // Initialize display
  display_init();

  // ----------------------------------------

  // Initialize timer0 at 0
  TCNT0 = 0;
  // Enable timer0
  TCCR0B = (1 << CS00);
  // Enable interrupts caused by timer0
  TIMSK0 = (1 << TOIE0);
  sei();

  // ----------------------------------------

  // Set backlight to white
  display_set_backlight_rgb(100, 100, 100);

  // Clear display
  display_clear();

  /*
  // Print message
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
  */

  // ----------------------------------------

  // Turn built-in LED off
  PORTB &= ~(1 << PORTB5);

  unsigned char button_state_curr = 0;
  unsigned char button_state_last = 0;
  unsigned int button_press_count = 0;

  // Print button press count
  display_place_cursor(0, 0);
  display_write_number(button_press_count);

  // Loop
  while(1) {

    // Light up built-in LED if button is being pressed
    if(button_state) {
      PORTB |= (1 << PORTB5);
    } else {
      PORTB &= ~(1 << PORTB5);
    }

    // Sample button state
    button_state_curr = button_state;
    // Compare current sample with last sample
    if(button_state_curr == 1 && button_state_last == 0) {
      // Increment button press count
      button_press_count++;
      // Print button press count
      display_place_cursor(0, 0);
      display_write_number(button_press_count);
    }
    // Update last sample to hold current state
    button_state_last = button_state_curr;

  }

  return 0;

}
