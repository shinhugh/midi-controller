// 2-wire serial interface (TWI)

#ifndef TWI_H
#define TWI_H

#include <stdint.h>

// --------------------------------------------------

uint8_t twi_transmit_start();

uint8_t twi_transmit_restart();

void twi_transmit_stop();

uint8_t twi_transmit_slaveaddr(uint8_t, uint8_t);

uint8_t twi_transmit_data(uint8_t);

uint8_t twi_receive_data_ack(uint8_t *);

uint8_t twi_receive_data_nack(uint8_t *);

// --------------------------------------------------

#endif
