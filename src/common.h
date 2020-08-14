// Common to all files

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

// --------------------------------------------------

// Pre-processor definitions

// CPU clock speed
#define F_CPU 16000000UL

// PWM duty cycle maximum value
#define PWM_MAX 255U

// --------------------------------------------------

// Global variables

// Elapsed time in milliseconds (reset at 2^16)
extern volatile uint16_t elapsed_ms;

// --------------------------------------------------

#endif
