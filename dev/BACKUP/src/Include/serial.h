#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "drv_uart.h"
#include "types.h"

void serial_init();

void serial_close();

void serial_write_byte(uint8_t data);

uint8_t serial_read_byte();

void serial_write_number(int32_t num);

void serial_write_string(const char *str);

int serial_read_string(char *str, uint32_t str_len);

void serial_write(const uint8_t *data, uint32_t data_len);

int serial_read(uint8_t *data, uint32_t data_len);

#endif /*__SCHED_H__*/