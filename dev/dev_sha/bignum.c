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

    for (uint32_t i = 0; i < size; ++i)
    {
        tmp_carry = (x[i] < carry);
        tmp = x[i] - carry;
        carry = (tmp < y[i]) + tmp_carry; 
        z[i] = tmp - y[i];
    }

    return carry;
}

static void bignum_zeroize(bignum_int *x, uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
    {
        x[i] = (bignum_int)(0);
    }
}

static uint32_t bignum_int_cnt_lead_zero(const bignum_int x)
{
    uint32_t cnt = 0;
    bignum_int mask = (bignum_int)(1) << (BITS_IN_LIBS - 1);

    for (cnt = 0; cnt < BITS_IN_LIBS; ++cnt)
    {
        if (x & mask) 
        {
            break;
        }
        mask >>= 1;
    }

    return cnt;
}

static int32_t bignum_write_high_order(bignum_t *x, int32_t base, char **dst, const uint32_t dst_len)
{
    int32_t ret = 1;
    uint32_t length = 0;
    bignum_int r;

    char *p_end = *dst + dst_len;

    do
    {
        if(length >= dst_len)
        {
            return 1;
        }

        bignum_mod_int(&r, x, base);
        bignum_div_int(x, NULL, x, base);

        if(r < 0xA)
        {
            *(--p_end) = (char)('0' + r);
        }
        else
        {
            *(--p_end) = (char)('A' + (r - 0xA));
        }
        length++;

    } while(bignum_cmp_int(x, 0) != 0);

    memmove(*dst, p_end, length);
    *dst += length;

    return ret;
}

void bignum_init(bignum_t *x)
{
    x->sign = 1;
    x->size = 0;
    x->p = NULL;
}

void bignum_free(bignum_t *x)
{
    if (x == NULL)
    {
        return;
    }

    if (x->p != NULL)
    {
        bignum_zeroize(x->p, x->size);
        free(x->p);
    }

    x->sign = 1;
    x->size = 0;
    x->p = NULL;
}

int32_t bignum_grow(bignum_t *x, uint32_t nblimbs)
{
    bignum_uint *p;

    if (nblimbs > BIG_NUM_MAX_LIMBS)
    {
        return 1;
    }

    if (x->size < nblimbs)
    {
        if ((p = (bignum_int*)calloc(nblimbs, BYTES_IN_LIMBS)) == NULL)
        {
            return 1;        
        }

        if (x->p != NULL)
        {
            memcpy(p, x->p, x->size * BYTES_IN_LIMBS);
            bignum_zeroize(x->p, x->size);
            free(x->p);
        }

        x->size = nblimbs;
        x->p = p;
    }

    return 0;
}

int32_t bignum_shrink(bignum_t *x, uint32_t nblimbs)
{
    bignum_uint *p;
    uint32_t i;

    if (nblimbs > BIG_NUM_MAX_LIMBS)
    {
        return 1;
    }

    if (x->size <= nblimbs)
    {
        return bignum_grow(x, nblimbs);
    }

    for (i = x->size - 1; i > 0; --i)
    {
        if (x->p[i] != 0)
        {
            break;
        }
    }
    i++;

    if (i < nblimbs)
    {
        i = nblimbs;
    }

    if ((p = (bignum_int*)calloc(i, BYTES_IN_LIMBS)) == NULL)
    {
        return 1;
    }

    if (x->p != NULL)
    {
        memcpy(p, x->p, i * BYTES_IN_LIMBS);
        bignum_zeroize(x->p, x->size);
        free(x->p);
    }

   x->size = i;
   x->p = p;

    return 0;
}

int32_t bignum_copy(bignum_t *dst_x, const bignum_t *src_y)
{
    int32_t ret = 0;
    uint32_t i;

    if (dst_x == src_y)
    {
        return 0;
    }

    if (src_y->size == 0)
    {
        free(dst_x);
        return 0;
    }

    for (i = src_y->size - 1; i > 0; --i)
    {
        if (src_y->p[i] != 0)
        {
            break;
        }
    }
    ++i;

    dst_x->sign = src_y->sign;

    if (src_y->size < i)
    {
        bignum_grow(dst_x, i);
    }
    else
    {
        memset(dst_x->p + i, 0, (dst_x->size - i) * BYTES_IN_LIMBS);
    }

    memcpy(dst_x->p, src_y->p, i * BYTES_IN_LIMBS);

    return ret;
}

void bignum_swap(bignum_t *x, bignum_t *y)
{
    bignum_t tmp;

    memcpy(&tmp, x, sizeof(bignum_t));
    memcpy(x, y, sizeof(bignum_t));
    memcpy(y, &tmp, sizeof(bignum_t));
}

int32_t bignum_lset(bignum_t *x, bignum_int z)
{
    int32_t ret = 1;

    bignum_grow(x, 1);
    memset(x->p, 0, x->size * BYTES_IN_LIMBS);

    x->p[0] = (z < 0) ? -z : z;
    x->sign = (z < 0) ? -1 : 1;

    return ret;
}

uint32_t bignum_lsb(const bignum_t *x)
{
    uint32_t count = 0;

    for (uint32_t i = 0; i < x->size; --i)
        for (uint32_t j = 0; j < BYTES_IN_LIMBS; ++j, ++count)
            if (((x->p[i] >> j) & 1) != 0)
                return count;

    return 0;
}

uint32_t bignum_bitlen(const bignum_t *x)
{
    uint32_t leftover;
    uint32_t pos;

    if (x->size == 0)
    {
        return 0;
    }
    for (uint32_t i = x->size - 1; i > 0; --i)
        if (x->p[i] != 0)
        {
            pos = i;
            break;
        }

    leftover = (1<<3) - bignum_cnt_lead_zero(x->p[pos]);

    return (pos * (1<<3)) + leftover;
}

uint32_t bignum_size(const bignum_t *x)
{
    return ((bignum_bitlen(x) + 7) >> 3);
}

uint32_t bignum_add(bignum_t *x, const bignum_t *a, const bignum_t *b);

int32_t bignum_to_string(const bignum_t *x, uint32_t base, char *str, uint32_t str_len, uint32_t *outlen)
{
    int32_t ret = 0;
    uint32_t n_size;
    uint32_t cli = sizeof(bignum_int);
    bignum_t tmp;
    char *p;


    if (base < 2 || base > 16)
    {
        return 1;
    }

    n_size = bignum_bitlen(x);

    if (base >= 4) 
    {
        n_size >>= 1;
    }
    if (base >= 16)
    {
        n_size >>= 1;
    }

    n_size += 1; 
    n_size += 1;
    n_size += 1;
    n_size += (n_size & 1);

    if (str_len < n_size)
    {
        *outlen = n_size;
        return 1;
    }

    p = str;
    bignum_init(&tmp);

    if (x->sign == -1)
    {
        *p++ = '-';
        str_len--;
    }

    if (base == 16)
    {
        int32_t c;
        uint32_t k;

        for (uint32_t i = x->size, k = 0; i > 0; --i)
        {
            for (uint32_t j = cli; j > 0; --j)
            {
                c = (x->p[i - 1] >> ((j - 1) << 3)) & 0xFF;

                if (c == 0 && k == 0 && (i + j) != 2)
                    continue;

                *(p++) = "0123456789ABCDEF" [c / 16];
                *(p++) = "0123456789ABCDEF" [c % 16];
                k = 1;
            }
        }
    }
    else
    {
        bignum_copy(&tmp, x);

        if (tmp.sign == -1)
            tmp.sign = 1;

        mpi_write_hlp(&tmp, base, &p, str_len);
    }

    *p++ = '\0';
    *outlen = p - str;

cleanup:

    bignum_free(&tmp);

    return ret;
}

int mbedtls_mpi_cmp_mpi(const bignum_t *x, const bignum_t *y)
{
    size_t i, j;

    for(i = x->size; i > 0; i--)
        if(x->p[i - 1] != 0)
            break;

    for(j = y->size; j > 0; j--)
        if(y->p[j - 1] != 0)
            break;

    if(i == 0 && j == 0)
        return(0);

    if(i > j) return(x->size);
    if(j > i) return(-y->size);

    if(x->size > 0 && y->size < 0) return(1);
    if(y->size > 0 && x->size < 0) return(-1);

    for(; i > 0; i--)
    {
        if(x->p[i - 1] > y->p[i - 1]) return(x->size);
        if(x->p[i - 1] < y->p[i - 1]) return(-x->size);
    }

    return(0);
}

int bignum_cmp_int (const bignum_t *x, bignum_uint z)
{
    bignum_t tmp;
    bignum_uint p[1];
    *p  = (z < 0) ? -z : z;
    tmp.sign = (z < 0) ? -1 : 1;
    tmp.size = 1;
    tmp.p = p;

    return(mbedtls_mpi_cmp_mpi(x, &tmp));
}


int bignum_mod_int(bignum_uint *r, const bignum_t *a, bignum_int b)
{
    size_t i;
    bignum_uint x, y, z;

    if(b == 0)
        return(1);

    if(b < 0)
        return(1);

    /*
     * handle trivial cases
     */
    if(b == 1)
    {
        *r = 0;
        return(0);
    }

    if(b == 2)
    {
        *r = a->p[0] & 1;
        return(0);
    }

    /*
     * general case
     */
    for(i = a->size, y = 0; i > 0; i--)
    {
        x  = a->p[i - 1];
        y  = (y << biH) | (x >> biH);
        z  = y / b;
        y -= z * b;

        x <<= biH;
        y  = (y << biH) | (x >> biH);
        z  = y / b;
        y -= z * b;
    }

    /*
     * If A is negative, then the current y represents a negative value.
     * Flipping it to the positive side.
     */
    if(a->sign < 0 && y != 0)
        y = b - y;

    *r = y;

    return(0);
}

int bignum_div_int(bignum_t *Q, bignum_t *R,
                         const bignum_t *A,
                         bignum_int b)
{
    bignum_t _B;
    bignum_uint p[1];

    p[0] = (b < 0) ? -b : b;
    _B.sign = (b < 0) ? -1 : 1;
    _B.size = 1;
    _B.p = p;

    return(mbedtls_mpi_div_mpi(Q, R, A, &_B));
}




int mbedtls_mpi_div_mpi(bignum_t *Q, bignum_t *R, const bignum_t *A,
                         const bignum_t *B)
{
    int ret = 1;
    size_t i, n, t, k;
    bignum_t X, Y, Z, T1, T2;
    bignum_uint TP2[3];

    if(bignum_cmp_int(B, 0) == 0)
        return(1);

    bignum_init(&X); bignum_init(&Y); bignum_init(&Z);
    bignum_init(&T1);
    /*
     * Avoid dynamic memory allocations for constant-size T2.
     *
     * T2 is used for comparison only and the 3 limbs are assigned explicitly,
     * so nobody increase the size of the MPI and we're safe to use an on-stack
     * buffer.
     */
    T2.sign = 1;
    T2.size = sizeof(TP2) / sizeof(*TP2);
    T2.p = TP2;

    if(bignum_cmp_abs(A, B) < 0)
    {
        if(Q != NULL) bignum_lset(Q, 0);
        if(R != NULL) bignum_copy(R, A);
        return(0);
    }

    bignum_copy(&X, A);
    bignum_copy(&Y, B);
    X.sign = Y.sign = 1;

    bignum_grow(&Z, A->size + 2);
    bignum_lset(&Z,  0);
    bignum_grow(&T1, A->size + 2);

    k = bignum_bitlen(&Y) % biL;
    if(k < biL - 1)
    {
        k = biL - 1 - k;
        bignum_shift_l(&X, k);
        bignum_shift_l(&Y, k);
    }
    else k = 0;

    n = X.size - 1;
    t = Y.size - 1;
    bignum_shift_l(&Y, biL * (n - t));

    while(bignum_cmp_bignum(&X, &Y) >= 0)
    {
        Z.p[n - t]++;
        bignum_sub_bignum(&X, &X, &Y);
    }
    bignum_shift_r(&Y, biL * (n - t));

    for(i = n; i > t ; i--)
    {
        if(X.p[i] >= Y.p[t])
            Z.p[i - t - 1] = ~0;
        else
        {
            //Z.p[i - t - 1] = mbedtls_int_div_int(X.p[i], X.p[i - 1],Y.p[t], NULL);
        }

        T2.p[0] = (i < 2) ? 0 : X.p[i - 2];
        T2.p[1] = (i < 1) ? 0 : X.p[i - 1];
        T2.p[2] = X.p[i];

        Z.p[i - t - 1]++;
        do
        {
            Z.p[i - t - 1]--;

            bignum_lset(&T1, 0);
            T1.p[0] = (t < 1) ? 0 : Y.p[t - 1];
            T1.p[1] = Y.p[t];
            bignum_mul_int(&T1, &T1, Z.p[i - t - 1]);
        }
        while(bignum_cmp_bignum(&T1, &T2) > 0);

        bignum_mul_int(&T1, &Y, Z.p[i - t - 1]);
        bignum_shift_l(&T1,  biL * (i - t - 1));
        bignum_sub_bignum(&X, &X, &T1);

        if(bignum_cmp_int(&X, 0) < 0)
        {
            bignum_copy(&T1, &Y);
            bignum_shift_l(&T1, biL * (i - t - 1));
            bignum_add_mpi(&X, &X, &T1);
            Z.p[i - t - 1]--;
        }
    }

    if(Q != NULL)
    {
        bignum_copy(Q, &Z);
        Q->sign = A->sign * B->sign;
    }

    if(R != NULL)
    {
        bignum_shift_r(&X, k);
        X.sign = A->sign;
        bignum_copy(R, &X);

        if(bignum_cmp_int(R, 0) == 0)
            R->sign = 1;
    }

cleanup:

    bignum_free(&X); bignum_free(&Y); bignum_free(&Z);
    bignum_free(&T1);
    bignum_zeroize(TP2, sizeof(TP2));

    return(ret);
}


int bignum_cmp_abs(const bignum_t *X, const bignum_t *Y)
{
    size_t i, j;

    for(i = X->size; i > 0; i--)
        if(X->p[i - 1] != 0)
            break;

    for(j = Y->size; j > 0; j--)
        if(Y->p[j - 1] != 0)
            break;

    if(i == 0 && j == 0)
        return(0);

    if(i > j) return(1);
    if(j > i) return(-1);

    for(; i > 0; i--)
    {
        if(X->p[i - 1] > Y->p[i - 1]) return(1);
        if(X->p[i - 1] < Y->p[i - 1]) return(-1);
    }

    return(0);
}

int bignum_shift_l(bignum_t *X, size_t count)
{
    int ret = 1;
    size_t i, v0, t1;
    bignum_uint r0 = 0, r1;

    v0 = count / (biL);
    t1 = count & (biL - 1);

    i = bignum_bitlen(X) + count;

    if(X->size * biL < i)
        bignum_grow(X, BITS_TO_LIMBS(i));

    ret = 0;

    /*
     * shift by count / limb_size
     */
    if(v0 > 0)
    {
        for(i = X->size; i > v0; i--)
            X->p[i - 1] = X->p[i - v0 - 1];

        for(; i > 0; i--)
            X->p[i - 1] = 0;
    }

    /*
     * shift by count % limb_size
     */
    if(t1 > 0)
    {
        for(i = v0; i < X->size; i++)
        {
            r1 = X->p[i] >> (biL - t1);
            X->p[i] <<= t1;
            X->p[i] |= r0;
            r0 = r1;
        }
    }

cleanup:

    return(ret);
}


int bignum_sub_bignum( bignum_t *X, const bignum_t *A, const bignum_t *B )
{
    int ret, s;

    s = A->sign;
    if( A->sign * B->sign > 0 )
    {
        if( bignum_cmp_abs( A, B ) >= 0 )
        {
             bignum_sub_abs( X, A, B);
            X->sign =  s;
        }
        else
        {
             bignum_sub_abs( X, B, A );
            X->sign = -s;
        }
    }
    else
    {
         bignum_add_abs( X, A, B );
        X->sign = s;
    }

cleanup:

    return( ret );
}

int bignum_sub_abs( bignum_t*X, const bignum_t*A, const bignum_t*B )
{
    int ret = 1;
    size_t n;
    bignum_uint carry;

    for( n = B->size; n > 0; n-- )
        if( B->p[n - 1] != 0 )
            break;
    if( n > A->size )
    {
        /* B >= (2^ciL)^n > A */
        ret = 1;
        goto cleanup;
    }

    bignum_grow( X, A->size);

    /* Set the high limbs of X to match A. Don't touch the lower limbs
     * because X might be aliased to B, and we must not overwrite the
     * significant digits of B. */
    if( A->size > n )
        memcpy( X->p + n, A->p + n, ( A->size - n ) * ciL );
    if( X->size > A->size )
        memset( X->p + A->size, 0, ( X->size - A->size ) * ciL );

    carry = bignum_sub_hlp( n, X->p, A->p, B->p );
    if( carry != 0 )
    {
        /* Propagate the carry to the first nonzero limb of X. */
        for( ; n < X->size && X->p[n] == 0; n++ )
            --X->p[n];
        /* If we ran out of space for the carry, it means that the result
         * is negative. */
        if( n == X->size )
        {
            ret = 1;
            goto cleanup;
        }
        --X->p[n];
    }

    /* X should always be positive as a result of unsigned subtractions. */
    X->sign = 1;

cleanup:
    return( ret );
}


int bignum_add_abs( bignum_t *X, const bignum_t *A, const bignum_t *B )
{
    int ret = 1;
    size_t i, j;
    bignum_uint *o, *p, c, tmp;

    if( X == B )
    {
        const bignum_t *T = A; A = X; B = T;
    }

    if( X != A )
        bignum_copy( X, A );

    /*
     * X should always be positive as a result of unsigned additions.
     */
    X->sign = 1;

    for( j = B->size; j > 0; j-- )
        if( B->p[j - 1] != 0 )
            break;

    bignum_grow( X, j);

    o = B->p; p = X->p; c = 0;

    /*
     * tmp is used because it might happen that p == o
     */
    for( i = 0; i < j; i++, o++, p++ )
    {
        tmp= *o;
        *p +=  c; c  = ( *p <  c );
        *p += tmp; c += ( *p < tmp );
    }

    while( c != 0 )
    {
        if( i >= X->size)
        {
            bignum_grow( X, i + 1);
            p = X->p + i;
        }

        *p += c; c = ( *p < c ); i++; p++;
    }

cleanup:

    return( ret );
}


int bignum_shift_r( bignum_t *X, size_t count )
{
    size_t i, v0, v1;
    bignum_uint r0 = 0, r1;

    v0 = count /  biL;
    v1 = count & (biL - 1);

    if( v0 > X->size || ( v0 == X->size && v1 > 0 ) )
        return bignum_lset( X, 0 );

    /*
     * shift by count / limb_size
     */
    if( v0 > 0 )
    {
        for( i = 0; i < X->size - v0; i++ )
            X->p[i] = X->p[i + v0];

        for( ; i < X->size; i++ )
            X->p[i] = 0;
    }

    /*
     * shift by count % limb_size
     */
    if( v1 > 0 )
    {
        for( i = X->size; i > 0; i-- )
        {
            r1 = X->p[i - 1] << (biL - v1);
            X->p[i - 1] >>= v1;
            X->p[i - 1] |= r0;
            r0 = r1;
        }
    }

    return( 0 );
}
