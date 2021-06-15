#ifndef __UTILS_H__
#define __UTILS_H__

#include "common_security.h"

void secure_clean(uint8_t *arr, uint32_t arr_len);
void print_arr(void *arr, int len);

#endif /*__UTILS_H__*/