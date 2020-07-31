// Serial communication via RX/TX ports

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

// --------------------------------------------------

void serial_print_newline();

void serial_print_binary(uint8_t);

void serial_print_hex(uint8_t);

void serial_print_number(uint64_t);

void serial_print_letter(char);

void serial_print_string(char *);

// --------------------------------------------------

#endif
