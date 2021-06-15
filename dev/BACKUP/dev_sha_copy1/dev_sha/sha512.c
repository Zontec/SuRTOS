#include "macros.h"

#include "sha512.h"
static const uint64_t K[80] =
{
    TO_U64(0x428A2F98D728AE22),  TO_U64(0x7137449123EF65CD),
    TO_U64(0xB5C0FBCFEC4D3B2F),  TO_U64(0xE9B5DBA58189DBBC),
    TO_U64(0x3956C25BF348B538),  TO_U64(0x59F111F1B605D019),
    TO_U64(0x923F82A4AF194F9B),  TO_U64(0xAB1C5ED5DA6D8118),
    TO_U64(0xD807AA98A3030242),  TO_U64(0x12835B0145706FBE),
    TO_U64(0x243185BE4EE4B28C),  TO_U64(0x550C7DC3D5FFB4E2),
    TO_U64(0x72BE5D74F27B896F),  TO_U64(0x80DEB1FE3B1696B1),
    TO_U64(0x9BDC06A725C71235),  TO_U64(0xC19BF174CF692694),
    TO_U64(0xE49B69C19EF14AD2),  TO_U64(0xEFBE4786384F25E3),
    TO_U64(0x0FC19DC68B8CD5B5),  TO_U64(0x240CA1CC77AC9C65),
    TO_U64(0x2DE92C6F592B0275),  TO_U64(0x4A7484AA6EA6E483),
    TO_U64(0x5CB0A9DCBD41FBD4),  TO_U64(0x76F988DA831153B5),
    TO_U64(0x983E5152EE66DFAB),  TO_U64(0xA831C66D2DB43210),
    TO_U64(0xB00327C898FB213F),  TO_U64(0xBF597FC7BEEF0EE4),
    TO_U64(0xC6E00BF33DA88FC2),  TO_U64(0xD5A79147930AA725),
    TO_U64(0x06CA6351E003826F),  TO_U64(0x142929670A0E6E70),
    TO_U64(0x27B70A8546D22FFC),  TO_U64(0x2E1B21385C26C926),
    TO_U64(0x4D2C6DFC5AC42AED),  TO_U64(0x53380D139D95B3DF),
    TO_U64(0x650A73548BAF63DE),  TO_U64(0x766A0ABB3C77B2A8),
    TO_U64(0x81C2C92E47EDAEE6),  TO_U64(0x92722C851482353B),
    TO_U64(0xA2BFE8A14CF10364),  TO_U64(0xA81A664BBC423001),
    TO_U64(0xC24B8B70D0F89791),  TO_U64(0xC76C51A30654BE30),
    TO_U64(0xD192E819D6EF5218),  TO_U64(0xD69906245565A910),
    TO_U64(0xF40E35855771202A),  TO_U64(0x106AA07032BBD1B8),
    TO_U64(0x19A4C116B8D2D0C8),  TO_U64(0x1E376C085141AB53),
    TO_U64(0x2748774CDF8EEB99),  TO_U64(0x34B0BCB5E19B48A8),
    TO_U64(0x391C0CB3C5C95A63),  TO_U64(0x4ED8AA4AE3418ACB),
    TO_U64(0x5B9CCA4F7763E373),  TO_U64(0x682E6FF3D6B2B8A3),
    TO_U64(0x748F82EE5DEFB2FC),  TO_U64(0x78A5636F43172F60),
    TO_U64(0x84C87814A1F0AB72),  TO_U64(0x8CC702081A6439EC),
    TO_U64(0x90BEFFFA23631E28),  TO_U64(0xA4506CEBDE82BDE9),
    TO_U64(0xBEF9A3F7B2C67915),  TO_U64(0xC67178F2E372532B),
    TO_U64(0xCA273ECEEA26619C),  TO_U64(0xD186B8C721C0C207),
    TO_U64(0xEADA7DD6CDE0EB1E),  TO_U64(0xF57D4F7FEE6ED178),
    TO_U64(0x06F067AA72176FBA),  TO_U64(0x0A637DC5A2C898A6),
    TO_U64(0x113F9804BEF90DAE),  TO_U64(0x1B710B35131C471B),
    TO_U64(0x28DB77F523047D84),  TO_U64(0x32CAAB7B40C72493),
    TO_U64(0x3C9EBE0A15C9BEBC),  TO_U64(0x431D67C49C100D4C),
    TO_U64(0x4CC5D4BECB3E42B6),  TO_U64(0x597F299CFC657E2A),
    TO_U64(0x5FCB6FAB3AD6FAEC),  TO_U64(0x6C44198C4A475817)
};

void sha512_init(sha512_t *ctx)
{
    memset(ctx, 0x00, sizeof(sha512_t));
    ctx->h0 = TO_U64(0x6A09E667F3BCC908);
    ctx->h1 = TO_U64(0xBB67AE8584CAA73B);
    ctx->h2 = TO_U64(0x3C6EF372FE94F82B);
    ctx->h3 = TO_U64(0xA54FF53A5F1D36F1);
    ctx->h4 = TO_U64(0x510E527FADE682D1);
    ctx->h5 = TO_U64(0x9B05688C2B3E6C1F);
    ctx->h6 = TO_U64(0x1F83D9ABFB41BD6B);
    ctx->h7 = TO_U64(0x5BE0CD19137E2179);
    
}

void sha512_process( sha512_t *ctx, const unsigned char data[128] )
{
    int i;
    uint64_t temp1, temp2, W[80];
    uint64_t A, B, C, D, E, F, G, H;

#define  SHR(x,n) (x >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (64 - n)))

#define S0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^  SHR(x, 7))
#define S1(x) (ROTR(x,19) ^ ROTR(x,61) ^  SHR(x, 6))

#define S2(x) (ROTR(x,28) ^ ROTR(x,34) ^ ROTR(x,39))
#define S3(x) (ROTR(x,14) ^ ROTR(x,18) ^ ROTR(x,41))

#define F0(x,y,z) ((x & y) | (z & (x | y)))
#define F1(x,y,z) (z ^ (x & (y ^ z)))

#define P(a,b,c,d,e,f,g,h,x,K)                  \
{                                               \
    temp1 = h + S3(e) + F1(e,f,g) + K + x;      \
    temp2 = S2(a) + F0(a,b,c);                  \
    d += temp1; h = temp1 + temp2;              \
}

    for( i = 0; i < 16; i++ )
    {
        GET_UINT64_BE( W[i], data, i << 3 );
    }

    for( ; i < 80; i++ )
    {
        W[i] = S1(W[i -  2]) + W[i -  7] +
               S0(W[i - 15]) + W[i - 16];
    }

    A = ctx->h0;
    B = ctx->h1;
    C = ctx->h2;
    D = ctx->h3;
    E = ctx->h4;
    F = ctx->h5;
    G = ctx->h6;
    H = ctx->h7;
    i = 0;

    do
    {
        P( A, B, C, D, E, F, G, H, W[i], K[i] ); i++;
        P( H, A, B, C, D, E, F, G, W[i], K[i] ); i++;
        P( G, H, A, B, C, D, E, F, W[i], K[i] ); i++;
        P( F, G, H, A, B, C, D, E, W[i], K[i] ); i++;
        P( E, F, G, H, A, B, C, D, W[i], K[i] ); i++;
        P( D, E, F, G, H, A, B, C, W[i], K[i] ); i++;
        P( C, D, E, F, G, H, A, B, W[i], K[i] ); i++;
        P( B, C, D, E, F, G, H, A, W[i], K[i] ); i++;
    }
    while( i < 80 );

    ctx->h0 += A;
    ctx->h1 += B;
    ctx->h2 += C;
    ctx->h3 += D;
    ctx->h4 += E;
    ctx->h5 += F;
    ctx->h6 += G;
    ctx->h7 += H;
}

void sha512_update(sha512_t *ctx, const uint8_t *data, uint32_t data_len)
{
    size_t fill;
    unsigned int left;

    if( data_len == 0 )
        return;

    left = (unsigned int) (ctx->total[0] & 0x7F);
    fill = 128 - left;

    ctx->total[0] += (uint64_t) data_len;

    if( ctx->total[0] < (uint64_t) data_len )
        ctx->total[1]++;

    if( left && data_len >= fill )
    {
        memcpy( (void *) (ctx->buffer + left), data, fill );
        sha512_process( ctx, ctx->buffer );
        data += fill;
        data_len  -= fill;
        left = 0;
    }

    while( data_len >= 128 )
    {
        sha512_process( ctx, data );
        data += 128;
        data_len  -= 128;
    }

    if( data_len > 0 )
        memcpy( (void *) (ctx->buffer + left), data, data_len );
}
static const unsigned char sha512_padding[128] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
void sha512_finish(sha512_t *ctx, uint8_t *out)
{
    size_t last, padn;
    uint64_t high, low;
    unsigned char msglen[16];

    high = ( ctx->total[0] >> 61 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );

    PUT_UINT64_BE( high, msglen, 0 );
    PUT_UINT64_BE( low,  msglen, 8 );

    last = (size_t)( ctx->total[0] & 0x7F );
    padn = ( last < 112 ) ? ( 112 - last ) : ( 240 - last );

    sha512_update( ctx, sha512_padding, padn );
    sha512_update( ctx, msglen, 16 );

    PUT_UINT64_BE( ctx->h0, out,  0 );
    PUT_UINT64_BE( ctx->h1, out,  8 );
    PUT_UINT64_BE( ctx->h2, out, 16 );
    PUT_UINT64_BE( ctx->h3, out, 24 );
    PUT_UINT64_BE( ctx->h4, out, 32 );
    PUT_UINT64_BE( ctx->h5, out, 40 );
    PUT_UINT64_BE( ctx->h6, out, 48 );
    PUT_UINT64_BE( ctx->h7, out, 56 );
    
}

void sha512(const uint8_t *data, uint32_t data_len, uint8_t *out)
{
    sha512_t ctx;
    sha512_init(&ctx);
    sha512_update(&ctx, data, data_len);
    sha512_finish(&ctx, out);
}