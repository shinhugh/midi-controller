#include "common.h"
#include <avr/io.h>
#include "serial_print.h"

// ASCII offset for numbers
#define CHAR_NUMERIC_OFFSET 0x30U
#define CHAR_HEX_OFFSET 0x61U

// ----------------------------------------

void serial_print_newline() {

  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) '\r';
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) '\n';

}

// ----------------------------------------

void serial_print_binary(uint8_t value) {

  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) 'b';
  uint8_t i = 7;
  while(1) {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = (uint8_t) (CHAR_NUMERIC_OFFSET + ((value >> i) & 0x01U));
    if(!i) {
      break;
    }
    i--;
  }

}

// ----------------------------------------

void serial_print_hex(uint8_t value) {

  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) '0';
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) 'x';
  if((value >> 4) > 0x09U) {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = (uint8_t) (CHAR_HEX_OFFSET + (value >> 4) - 0x0aU);
  } else {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = (uint8_t) (CHAR_NUMERIC_OFFSET + (value >> 4));
  }
  if((value & 0x0f) > 0x09U) {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = (uint8_t) (CHAR_HEX_OFFSET + (value & 0x0f) - 0x0aU);
  } else {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = (uint8_t) (CHAR_NUMERIC_OFFSET + (value & 0x0f));
  }

}

// ----------------------------------------

void serial_print_number(uint64_t number) {

  uint8_t i;

  uint8_t length = 0;
  uint64_t number_copy = number;
  while(number_copy > 0) {
    length++;
    number_copy /= 10;
  }
  length = length == 0 ? 1 : length;

  uint8_t curr_digit;
  uint64_t divisor = 1;
  for(i = 0; i < length - 1; i++) {
    divisor *= 10;
  }
  for(i = 0; i < length; i++) {
    curr_digit = (uint8_t) ((number / divisor) % 10);
    divisor /= 10;
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = (uint8_t) (CHAR_NUMERIC_OFFSET + curr_digit);
  }

}

// ----------------------------------------

void serial_print_letter(char letter) {

  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (uint8_t) letter;

}

// ----------------------------------------

void serial_print_string(char *string) {

  while(*string) {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = (uint8_t) *string;
    string++;
  }

}
