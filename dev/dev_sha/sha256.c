#include "sha256.h"

static const uint8_t sha256_padding[SHA256_BUFFER_SIZE] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const uint32_t K[SHA256_BUFFER_SIZE] =
{
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
    0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
    0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
    0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
    0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
    0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
    0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
    0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
    0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
};

#define SHR(x, n) ((x & 0xFFFFFFFF) >> n)
#define ROTR(x, n) (SHR(x, n) | (x << (32 - n)))

#define S0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^  SHR(x, 3))
#define S1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^  SHR(x, 10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define F0(x, y, z) ((x & y) | (z & (x | y)))
#define F1(x, y, z) (z ^ (x & (y ^ z)))

#define R(t)                                    \
(                                              \
    W[t] = S1(W[t -  2]) + W[t -  7] +          \
           S0(W[t - 15]) + W[t - 16]            \
)

#define P(a, b, c, d, e, f, g, h, x, K)         \
{                                               \
    temp1 = h + S3(e) + F1(e,f,g) + K + x;      \
    temp2 = S2(a) + F0(a,b,c);                  \
    d += temp1; h = temp1 + temp2;              \
}

static void sha256_process(sha256_t *ctx, const uint8_t *data)
{
    uint32_t temp1;
    uint32_t temp2;
    uint32_t W[SHA256_BUFFER_SIZE];
    uint32_t A[8];
    uint32_t i;

    A[0] = ctx->h0;
    A[1] = ctx->h1;
    A[2] = ctx->h2;
    A[3] = ctx->h3;
    A[4] = ctx->h4;
    A[5] = ctx->h5;
    A[6] = ctx->h6;
    A[7] = ctx->h7;

    for(i = 0; i < 16; ++i)
    {
        GET_UINT32_BE(W[i], data, 4 * i);
    }

    for(i = 0; i < 16; i += 8)
    {
        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i+0], K[i+0]);
        P(A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], W[i+1], K[i+1]);
        P(A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], W[i+2], K[i+2]);
        P(A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], W[i+3], K[i+3]);
        P(A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], W[i+4], K[i+4]);
        P(A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], W[i+5], K[i+5]);
        P(A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], W[i+6], K[i+6]);
        P(A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], W[i+7], K[i+7]);
    }

    for(i = 16; i < 64; i += 8)
    {
        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], R(i+0), K[i+0]);
        P(A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], R(i+1), K[i+1]);
        P(A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], R(i+2), K[i+2]);
        P(A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], R(i+3), K[i+3]);
        P(A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], R(i+4), K[i+4]);
        P(A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], R(i+5), K[i+5]);
        P(A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], R(i+6), K[i+6]);
        P(A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], R(i+7), K[i+7]);
    }

    ctx->h0 += A[0];
    ctx->h1 += A[1];
    ctx->h2 += A[2];
    ctx->h3 += A[3];
    ctx->h4 += A[4];
    ctx->h5 += A[5];
    ctx->h6 += A[6];
    ctx->h7 += A[7];
}

security_status_e sha256_init(sha256_t *ctx)
{
    _SECURITY_FUNCTION_BEGIN;

    _SECURITY_CHECK_VALID_NOT_NULL(ctx);

    _SECURITY_CHECK_VALID_NOT_NULL(memset(ctx, 0x00, sizeof(sha256_t)));

    ctx->h0 = 0x6A09E667;
    ctx->h1 = 0xBB67AE85;
    ctx->h2 = 0x3C6EF372;
    ctx->h3 = 0xA54FF53A;
    ctx->h4 = 0x510E527F;
    ctx->h5 = 0x9B05688C;
    ctx->h6 = 0x1F83D9AB;
    ctx->h7 = 0x5BE0CD19;

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
}

security_status_e sha256_update(sha256_t *ctx, const uint8_t *data, uint32_t data_len)
{
    _SECURITY_FUNCTION_BEGIN;

    uint32_t fill;
    uint32_t left;

    _SECURITY_CHECK_VALID_NOT_NULL(data);
    _SECURITY_CHECK_VALID_NOT_NULL(ctx);

    if (data_len == 0)
    {
        goto _SECURITY_EXIT;
    }

    left = ctx->total[0] & 0x3F;
    fill = SHA256_BUFFER_SIZE - left;

    ctx->total[0] += data_len;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < data_len)
    {
        ctx->total[1]++;
    }

    if (left && data_len >= fill)
    {
        _SECURITY_CHECK_VALID_NOT_NULL(memcpy((void *)(ctx->buffer + left), data, fill));
        sha256_process(ctx, ctx->buffer);
        data += fill;
        data_len -= fill;
        left = 0;
    }

    while (data_len >= SHA256_BUFFER_SIZE)
    {
        sha256_process(ctx, data);
        data += SHA256_BUFFER_SIZE;
        data_len  -= SHA256_BUFFER_SIZE;
    }

    if (data_len > 0)
    {
        _SECURITY_CHECK_VALID_NOT_NULL(memcpy((void *)(ctx->buffer + left), data, data_len));
    }

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
}

security_status_e sha256_finish(sha256_t *ctx, uint8_t *out)
{
    _SECURITY_FUNCTION_BEGIN;

    uint32_t last, padn;
    uint32_t high, low;
    uint8_t msglen[8];

    _SECURITY_CHECK_VALID_NOT_NULL(ctx);
    _SECURITY_CHECK_VALID_NOT_NULL(out);

    high = (ctx->total[0] >> 29)
         | (ctx->total[1] << 3);
    low  = (ctx->total[0] << 3);

    PUT_UINT32_BE(high, msglen, 0);
    PUT_UINT32_BE(low, msglen, 4);

    last = ctx->total[0] & 0x3F;
    padn = (last < 56) ? (56 - last) : (120 - last);

    _SECURITY_VALID_RES(sha256_update(ctx, sha256_padding, padn));
    _SECURITY_VALID_RES(sha256_update(ctx, msglen, 8));

    PUT_UINT32_BE(ctx->h0, out, 0);
    PUT_UINT32_BE(ctx->h1, out, 4);
    PUT_UINT32_BE(ctx->h2, out, 8);
    PUT_UINT32_BE(ctx->h3, out, 12);
    PUT_UINT32_BE(ctx->h4, out, 16);
    PUT_UINT32_BE(ctx->h5, out, 20);
    PUT_UINT32_BE(ctx->h6, out, 24);
    PUT_UINT32_BE(ctx->h7, out, 28);

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
}

security_status_e sha256(const uint8_t *data, uint32_t data_len, uint8_t *out)
{
    _SECURITY_FUNCTION_BEGIN;

    sha256_t ctx;

    _SECURITY_VALID_RES(sha256_init(&ctx));
    _SECURITY_VALID_RES(sha256_update(&ctx, data, data_len));
    _SECURITY_VALID_RES(sha256_finish(&ctx, out));

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
}