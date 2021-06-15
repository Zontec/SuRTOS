#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <stdio.h>
#include <stdint.h>

typedef uint32_t bignum_uint;
typedef int32_t bignum_int;

#define BIG_NUM_MAX_LIMBS 1000

typedef struct bignum_t
{
    int32_t sign;
    uint32_t size;
    bignum_uint *p;
} bignum_t;

void bignum_init(bignum_t *x);

void bignum_free(bignum_t *x);

int32_t bignum_grow(bignum_t *x, uint32_t nblimbs);

int32_t bignum_shrink(bignum_t *x, uint32_t nblimbs);

int32_t bignum_copy(bignum_t *dst_x, const bignum_t *src_x);

void bignum_swap(bignum_t *x, bignum_t *y);

int32_t bignum_lset(bignum_t *x, bignum_int z);

uint32_t bignum_lsb(const bignum_t *x);

uint32_t bignum_bitlen(const bignum_t *x);

uint32_t bignum_size(const bignum_t *x);

uint32_t bignum_add(bignum_t *x, const bignum_t *a, const bignum_t *b);

char *bignum_to_dec_string(const bignum_t *x);

char *bignum_to_hex_string(const bignum_t *x);

#endif /*__BIGNUM_H__*/