#include "common.h"
#include <avr/io.h>
#include "twi.h"

// --------------------------------------------------

uint8_t twi_transmit_start() {

  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while(!(TWCR & (1 << TWINT)));
  if(TWSR != 0x08) {
    return 1;
  }
  return 0;

}

// --------------------------------------------------

uint8_t twi_transmit_restart() {

  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while(!(TWCR & (1 << TWINT)));
  if(TWSR != 0x10) {
    return 1;
  }
  return 0;

}

// --------------------------------------------------

void twi_transmit_stop() {

  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

}

// --------------------------------------------------

uint8_t twi_transmit_slaveaddr(uint8_t slave_addr, uint8_t read_mode) {

  slave_addr = slave_addr << 1;

  // If receiver
  if(read_mode) {
    TWDR = slave_addr | 0x01;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));
    if(TWSR != 0x40) {
      return 1;
    }
  }

  // If transmitter
  else {
    TWDR = slave_addr;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));
    if(TWSR != 0x18) {
      return 1;
    }
  }

  return 0;

}

// --------------------------------------------------

uint8_t twi_transmit_data(uint8_t data) {

  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while(!(TWCR & (1 << TWINT)));
  if(TWSR != 0x28) {
    return 1;
  }
  return 0;

}

// --------------------------------------------------

uint8_t twi_receive_data_ack(uint8_t *data) {

  TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
  while(!(TWCR & (1 << TWINT)));
  if(TWSR != 0x50) {
    return 1;
  }

  if(data) {
    *data = TWDR;
  }

  return 0;

}

// --------------------------------------------------

uint8_t twi_receive_data_nack(uint8_t *data) {

  TWCR = (1 << TWINT) | (1 << TWEN);
  while(!(TWCR & (1 << TWINT)));
  if(TWSR != 0x58) {
    return 1;
  }

  if(data) {
    *data = TWDR;
  }

  return 0;

}
