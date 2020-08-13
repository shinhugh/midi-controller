// Interface to I/O expander

#ifndef IO_EXPAND_H
#define IO_EXPAND_H

/*
 * MCP23017
 */

// --------------------------------------------------

void io_expand_init(uint8_t);

uint16_t io_expand_read_bytes(uint8_t);

// --------------------------------------------------

#endif
