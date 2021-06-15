#include "bignum.h"

#define BYTES_IN_LIMBS         sizeof(bignum_int)
#define BITS_IN_LIBS           (BYTES_IN_LIMBS << 3)
#define BITS_IN_HALF_LIBS      (BYTES_IN_LIMBS << 2)

/*
* z = x - y;
*/
static bignum_uint bignum_sub_bignum_with_eq_size(bignum_uint *z, const bignum_uint *x, 
                                                    const bignum_uint *y, uint32_t size)
{
    bignum_uint carry = 0;
    bignum_uint tmp_carry;
    bignum_uint tmp;

    for(uint32_t i = 0; i < size; i++ )
    {
        tmp_carry = (x[i] < carry);
        tmp = x[i] - carry;
        carry = (tmp < y[i]) + tmp_carry; 
        z[i] = tmp - y[i];
    }

    return carry;
}