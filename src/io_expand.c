#include "common.h"
#include <avr/io.h>
#include "io_expand.h"
#include "twi.h"

// --------------------------------------------------

void io_expand_init(uint8_t addr) {

  addr &= 0x07;
  addr += 0x20;

  // Transmit start condition
  twi_transmit_start();
  // Transmit slave address + write
  twi_transmit_slaveaddr(addr, 0);
  // Transmit register address of IOCON
  twi_transmit_data(0x0a);
  // Transmit IOCON value
  twi_transmit_data(0x00);

  // Transmit restart condition
  twi_transmit_restart();
  // Transmit slave address + write
  twi_transmit_slaveaddr(addr, 0);
  // Transmit register address of IODIRA
  twi_transmit_data(0x00);
  // Transmit IODIRA value
  twi_transmit_data(0xff);
  // Transmit IODIRB value
  twi_transmit_data(0xff);

  // Transmit restart condition
  twi_transmit_restart();
  // Transmit slave address + write
  twi_transmit_slaveaddr(addr, 0);
  // Transmit register address of GPPUA
  twi_transmit_data(0x0c);
  // Transmit GPPUA value
  twi_transmit_data(0xff);
  // Transmit GPPUB value
  twi_transmit_data(0xff);

  // Transmit restart condition
  twi_transmit_restart();
  // Transmit slave address + write
  twi_transmit_slaveaddr(addr, 0);
  // Transmit register address of IPOLA
  twi_transmit_data(0x02);
  // Transmit IPOLA value
  twi_transmit_data(0x00);
  // Transmit IPOLB value
  twi_transmit_data(0x00);

  // Transmit stop condition
  twi_transmit_stop();

}

// --------------------------------------------------

uint16_t io_expand_read_bytes(uint8_t addr) {

  uint16_t data;

  addr &= 0x07;
  addr += 0x20;

  // Transmit start condition
  twi_transmit_start();
  // Transmit slave address + write
  twi_transmit_slaveaddr(addr, 0);
  // Transmit register address of GPIOA
  twi_transmit_data(0x12);
  // Transmit restart condition
  twi_transmit_restart();
  // Transmit slave address + read
  twi_transmit_slaveaddr(addr, 1);
  // Proceed to receive data byte, then transmit ACK
  twi_receive_data_ack(0);

  // Place as MSB
  data = ~TWDR;
  data = data << 8;

  // Proceed to receive data byte, then transmit NACK
  twi_receive_data_nack(0);

  // Place as LSB
  data |= ~TWDR;

  // Transmit stop condition
  twi_transmit_stop();

  // Return data
  return data;

}

// --------------------------------------------------

// TODO

// --------------------------------------------------

// TODO
