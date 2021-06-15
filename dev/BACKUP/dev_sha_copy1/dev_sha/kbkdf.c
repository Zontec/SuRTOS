#include "kbkdf.h"
#include "macros.h"

#define HMAC_MAX_OUTPUT (64)
#define BITS_IN_BYTE (8)
#define RLEN (4) // RLEN is limited with uint32_t type

#ifndef BIT_MASK_LEFT
static uint8_t _bit_mask_left[] = {
    0,
    0x80,
    0x80 + 0x40,
    0x80 + 0x40 + 0x20,
    0x80 + 0x40 + 0x20 + 0x10,
    0x80 + 0x40 + 0x20 + 0x10 + 0x08,
    0x80 + 0x40 + 0x20 + 0x10 + 0x08 + 0x04,
    0x80 + 0x40 + 0x20 + 0x10 + 0x08 + 0x04 + 0x02,
    0x80 + 0x40 + 0x20 + 0x10 + 0x08 + 0x04 + 0x02 + 0x01,
};
#define BIT_MASK_LEFT(n) (_bit_mask_left[n])
#endif

#ifndef BIT_MASK_RIGHT
static uint8_t _bit_mask_right[] = {
    0,
    0x01,
    0x02 + 0x01,
    0x04 + 0x02 + 0x01,
    0x08 + 0x04 + 0x02 + 0x01,
    0x10 + 0x08 + 0x04 + 0x02 + 0x01,
    0x20 + 0x10 + 0x08 + 0x04 + 0x02 + 0x01,
    0x40 + 0x20 + 0x10 + 0x08 + 0x04 + 0x02 + 0x01,
    0x80 + 0x40 + 0x20 + 0x10 + 0x08 + 0x04 + 0x02 + 0x01,
};
#define BIT_MASK_RIGHT(n) (_bit_mask_right[n])
#endif

static uint32_t kbkdf_hash_size[] =
{
    32, //KBKDF_HASH_TYPE_SHA384
    48, //KBKDF_HASH_TYPE_SHA384
    64  //KBKDF_HASH_TYPE_SHA512
};

static void _shift_array_right(uint8_t *array, uint32_t length, uint32_t shift, uint8_t carry) // 1 <= shift <= 7
{
    uint8_t temp;

    for (uint32_t i = 0; i < length; i++)
    {
        temp = array[i];                        // save the value
        array[i] = (array[i] >> shift) | carry; // update the array element
        carry = temp << (8 - shift);            // compute the new carry
    }
}

static uint32_t _kbkdf_counter(kbkdf_hash_type_e hash_type, kbkdf_hmac_callbacks_t hmac_callbacks,
                               const uint8_t *key_in, const uint32_t key_in_len,
                               uint8_t *fixed_input, const uint32_t fixed_input_len,
                               uint8_t *key_out, const uint32_t key_out_len,
                               kbkdf_opts_t *opts)
{

    hmac_init hmac_init = hmac_callbacks.hmac_init;
    hmac_update hmac_update = hmac_callbacks.hmac_update;
    hmac_final hmac_final = hmac_callbacks.hmac_final;

    uint32_t status = SECURITY_STATUS_FAIL;
    uint32_t hmac_size;
    uint32_t full_blocks;
    uint32_t leftover;
    uint32_t modulo;
    uint32_t rpos;
    uint32_t rpos_fixed;
    uint8_t rpos_modulo;

    uint8_t *key_out_last;
    uint8_t ctr[RLEN + 1];

    if (opts->ctr_rlen > RLEN)
    {
        return status;
    }

    hmac_size = kbkdf_hash_size[hash_type];

    rpos_modulo = opts->ctr_rpos % 8;
    rpos = opts->ctr_rpos / 8;
    rpos_fixed = rpos + ((rpos_modulo > 0) ? 1 : 0);

    modulo = ((key_out_len % 8) > 1) ? 1 : 0;
    full_blocks = ((key_out_len / 8) + modulo) / hmac_size;
    leftover = ((key_out_len / 8) + modulo) % hmac_size;
    key_out_last = key_out;

    for (uint32_t i = 1; i <= full_blocks; ++i, key_out += hmac_size)
    {
        PUT_UINT32_BE(i, ctr, 0);
        ctr[4] = 0;

        if (hmac_init(key_in, key_in_len) != SECURITY_STATUS_OK)
        {
            goto exit;
        }

        if (rpos > 0)
        {
            if (hmac_update(fixed_input, rpos) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if (opts->ctr_rlen > 0)
        {
            if (rpos_modulo > 0)
            {
                _shift_array_right(ctr + (RLEN - opts->ctr_rlen), 
                                    opts->ctr_rlen + 1, rpos_modulo, 
                                    (fixed_input[rpos] & BIT_MASK_LEFT(rpos_modulo)));
                ctr[4] |= fixed_input[rpos] & BIT_MASK_RIGHT(8 - rpos_modulo);
            }
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), 
                            opts->ctr_rlen + ((rpos_modulo > 0) ? 1 : 0)) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if ((rpos_fixed != fixed_input_len) &&
            (hmac_update(fixed_input + rpos_fixed, fixed_input_len - rpos_fixed) != SECURITY_STATUS_OK))
        {
            goto exit;
        }

        if (hmac_final(key_out) != SECURITY_STATUS_OK)
        {
            goto exit;
        }
        key_out_last = key_out;
    }
    if (leftover)
    {
        uint8_t last_block[HMAC_MAX_OUTPUT];

        PUT_UINT32_BE(full_blocks + 1, ctr, 0);
        ctr[4] = 0;

        if (hmac_init(key_in, key_in_len) != SECURITY_STATUS_OK)
        {
            goto exit;
        }

        if (rpos > 0)
        {
            if (hmac_update(fixed_input, rpos) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if (opts->ctr_rlen > 0)
        {
            if (rpos_modulo > 0)
            {
                _shift_array_right(ctr + (RLEN - opts->ctr_rlen), 
                                    opts->ctr_rlen + 1, rpos_modulo, 
                                    (fixed_input[rpos] & BIT_MASK_LEFT(rpos_modulo)));
                ctr[4] |= fixed_input[rpos] & BIT_MASK_RIGHT(8 - rpos_modulo);
            }
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), 
                            opts->ctr_rlen + ((rpos_modulo > 0) ? 1 : 0)) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if ((rpos_fixed != fixed_input_len) &&
            (hmac_update(fixed_input + rpos_fixed, 
                        fixed_input_len - rpos_fixed) != SECURITY_STATUS_OK))
        {
            goto exit;
        }

        if (hmac_final(last_block) != SECURITY_STATUS_OK)
        {
            goto exit;
        }

        memcpy(key_out, last_block, leftover);
        memset(last_block, 0xFF, HMAC_MAX_OUTPUT);

        if (modulo)
        {
            key_out[leftover - 1] &= BIT_MASK_LEFT(key_out_len % 8);
        }
    }
    else
    {
        if (modulo)
        {
            key_out_last[hmac_size - 1] &= BIT_MASK_LEFT(key_out_len % 8);
        }
    }

    status = SECURITY_STATUS_OK;
exit:

    return status;
}

static uint32_t _kbkdf_feedback(kbkdf_hash_type_e hash_type, kbkdf_hmac_callbacks_t hmac_callbacks,
                                const uint8_t *key_in, const uint32_t key_in_len,
                                const uint8_t *iv_in, const uint32_t iv_in_len,
                                uint8_t *fixed_input, const uint32_t fixed_input_len,
                                uint8_t *key_out, const uint32_t key_out_len,
                                kbkdf_opts_t *opts)
{

    hmac_init hmac_init = hmac_callbacks.hmac_init;
    hmac_update hmac_update = hmac_callbacks.hmac_update;
    hmac_final hmac_final = hmac_callbacks.hmac_final;

    uint32_t status = SECURITY_STATUS_FAIL;

    const uint8_t *k_i_1 = iv_in;
    uint32_t k_i_1_len = 0;

    uint32_t hmac_size;
    uint32_t full_blocks;
    uint32_t leftover;
    uint32_t modulo;
    uint8_t ctr[RLEN + 1];
    uint8_t *key_out_last;

    if (opts->ctr_rlen > RLEN)
    {
        return status;
    }

    hmac_size = kbkdf_hash_size[hash_type];

    modulo = ((key_out_len % 8) > 1) ? 1 : 0;
    full_blocks = ((key_out_len / 8) + modulo) / hmac_size;
    leftover = ((key_out_len / 8) + modulo) % hmac_size;

    key_out_last = key_out;

    if (iv_in_len > 0)
    {
        k_i_1_len = iv_in_len;
    }
    for (uint32_t i = 1; i <= full_blocks; ++i, key_out += hmac_size)
    {
        PUT_UINT32_BE(i, ctr, 0);
        ctr[4] = 0;

        if (hmac_init(key_in, key_in_len) != SECURITY_STATUS_OK)
        {
            goto exit;
        }
        if ((opts->ctr_rpos == -1) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if ((k_i_1_len > 0) && (hmac_update(k_i_1, k_i_1_len) != SECURITY_STATUS_OK))
        {
            goto exit;
        }

        if ((opts->ctr_rpos == 0) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if ((opts->ctr_rpos > 0) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(fixed_input, opts->ctr_rpos / 8) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
            if ((opts->ctr_rpos != (int64_t)fixed_input_len) &&
                (hmac_update(fixed_input + opts->ctr_rpos / 8, 
                            fixed_input_len - opts->ctr_rpos / 8) != SECURITY_STATUS_OK))
            {
                goto exit;
            }
        }
        else
        {
            if (hmac_update(fixed_input, fixed_input_len) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if (hmac_final(key_out) != SECURITY_STATUS_OK)
        {
            goto exit;
        }

        k_i_1 = key_out;
        k_i_1_len = hmac_size;
        key_out_last = key_out;
    }
    if (leftover)
    {
        uint8_t last_block[HMAC_MAX_OUTPUT];

        PUT_UINT32_BE(full_blocks + 1, ctr, 0);
        ctr[4] = 0;

        if (hmac_init(key_in, key_in_len) != SECURITY_STATUS_OK)
        {
            goto

                exit;
        }
        if ((opts->ctr_rpos == -1) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        // K(i) := PRF (KI, I_V | Ki-1)
        if ((k_i_1_len > 0) && (hmac_update(k_i_1, k_i_1_len) != SECURITY_STATUS_OK))
        {
            goto exit;
        }
        if ((opts->ctr_rpos == 0) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if ((opts->ctr_rpos > 0) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(fixed_input, opts->ctr_rpos / 8) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
            if ((opts->ctr_rpos != (int64_t)fixed_input_len) &&
                (hmac_update(fixed_input + opts->ctr_rpos / 8, 
                            fixed_input_len - opts->ctr_rpos / 8) != SECURITY_STATUS_OK))
            {
                goto exit;
            }
        }
        else
        {
            if (hmac_update(fixed_input, fixed_input_len) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if (hmac_final(last_block) != SECURITY_STATUS_OK)
        {
            goto exit;
        }

        memcpy(key_out, last_block, leftover);
        memset(last_block, 0xFF, HMAC_MAX_OUTPUT);

        if (modulo)
        {
            key_out[leftover - 1] &= BIT_MASK_LEFT(key_out_len % 8);
        }
    }
    else
    {
        if (modulo)
        {
            key_out_last[hmac_size - 1] &= BIT_MASK_LEFT(key_out_len % 8);
        }
    }

    status = SECURITY_STATUS_OK;
exit:

    return status;
}

static uint32_t _kbkdf_double_pipeline(kbkdf_hash_type_e hash_type, kbkdf_hmac_callbacks_t hmac_callbacks,
                                       const uint8_t *key_in, const uint32_t key_in_len,
                                       uint8_t *fixed_input, const uint32_t fixed_input_len,
                                       uint8_t *key_out, const uint32_t key_out_len,
                                       kbkdf_opts_t *opts)
{

    hmac_init hmac_init = hmac_callbacks.hmac_init;
    hmac_update hmac_update = hmac_callbacks.hmac_update;
    hmac_final hmac_final = hmac_callbacks.hmac_final;

    uint32_t status = SECURITY_STATUS_FAIL;

    uint32_t hmac_size;
    uint32_t full_blocks;
    uint32_t leftover;
    uint32_t A_len;
    uint32_t modulo;

    uint8_t A[HMAC_MAX_OUTPUT];
    uint8_t ctr[RLEN + 1];
    uint8_t *key_out_last;
    void *A_i_1;
    void *A_i;

    if (opts->ctr_rlen > RLEN)
    {
        return SECURITY_STATUS_FAIL;
    }

    A_i_1 = fixed_input;
    A_len = fixed_input_len;

    A_i = &A[0];

    hmac_size = kbkdf_hash_size[hash_type];

    modulo = ((key_out_len % 8) > 1) ? 1 : 0;
    full_blocks = ((key_out_len / 8) + modulo) / hmac_size;
    leftover = ((key_out_len / 8) + modulo) % hmac_size;

    key_out_last = key_out;

    for (uint32_t i = 1; i <= full_blocks; ++i, key_out += hmac_size)
    {
        PUT_UINT32_BE(i, ctr, 0);
        ctr[4] = 0;

        if (hmac_init(key_in, key_in_len) != SECURITY_STATUS_OK)
        {
            goto exit;
        }
        if ((hmac_update(A_i_1, A_len) != SECURITY_STATUS_OK))
        {
            goto exit;
        }
        if ((hmac_final(A_i) != SECURITY_STATUS_OK))
        {
            goto exit;
        }

        if (hmac_init(key_in, key_in_len) != SECURITY_STATUS_OK)
        {
            goto exit;
        }
        if ((hmac_update(A_i, hmac_size) != SECURITY_STATUS_OK))
        {
            goto exit;
        }
        if ((opts->ctr_rpos == -1) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }

        if ((opts->ctr_rpos == 0) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }

        if ((opts->ctr_rpos > 0) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(fixed_input, opts->ctr_rpos / 8) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }

            if ((opts->ctr_rpos != (int64_t)fixed_input_len) &&
                (hmac_update(fixed_input +
                            opts->ctr_rpos / 8,
                            fixed_input_len - opts->ctr_rpos / 8) != SECURITY_STATUS_OK))
            {
                goto exit;
            }
        }
        else
        {
            if (hmac_update(fixed_input, fixed_input_len) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if (hmac_final(key_out) != SECURITY_STATUS_OK)
        {
            goto exit;
        }

        A_i_1 = &A[0];
        A_len = hmac_size;
        key_out_last = key_out;
    }
    if (leftover)
    {
        uint8_t last_block[HMAC_MAX_OUTPUT];

        PUT_UINT32_BE(full_blocks + 1, ctr, 0);
        ctr[4] = 0;

        if (hmac_init(key_in, key_in_len) != SECURITY_STATUS_OK)
        {
            goto exit;
        }
        if ((hmac_update(A_i_1, A_len) != SECURITY_STATUS_OK))
        {
            goto exit;
        }
        if ((hmac_final(A_i) != SECURITY_STATUS_OK))
        {
            goto exit;
        }

        if (hmac_init(key_in, key_in_len) != SECURITY_STATUS_OK)
        {
            goto exit;
        }

        if ((opts->ctr_rpos == -1) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if ((hmac_update(A_i, A_len) != SECURITY_STATUS_OK))
        {
            goto exit;
        }

        if ((opts->ctr_rpos == 0) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }

        if ((opts->ctr_rpos > 0) &&
            (opts->ctr_rlen > 0))
        {
            if (hmac_update(fixed_input, opts->ctr_rpos / 8) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
            if (hmac_update(ctr + (RLEN - opts->ctr_rlen), opts->ctr_rlen) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
            if ((opts->ctr_rpos != (int64_t)fixed_input_len) &&
                (hmac_update(fixed_input + opts->ctr_rpos / 8, 
                            fixed_input_len - opts->ctr_rpos / 8) != SECURITY_STATUS_OK))
            {
                goto exit;
            }
        }
        else
        {
            if (hmac_update(fixed_input, fixed_input_len) != SECURITY_STATUS_OK)
            {
                goto exit;
            }
        }
        if (hmac_final(last_block) != SECURITY_STATUS_OK)
        {
            goto exit;
        }

        memcpy(key_out, last_block, leftover);
        memset(last_block, 0xFF, HMAC_MAX_OUTPUT);

        if (modulo)
        {
            key_out[leftover - 1] &= BIT_MASK_LEFT(key_out_len % 8);
        }
    }
    else
    {
        if (modulo)
        {
            key_out_last[hmac_size - 1] &= BIT_MASK_LEFT(key_out_len % 8);
        }
    }

    status = SECURITY_STATUS_OK;
exit:

    return status;
}

security_status_e kbkdf(kbkdf_mode_e mode, kbkdf_hash_type_e hash_type,
                        kbkdf_hmac_callbacks_t hmac_callbacks,
                        const uint8_t *key_in, const uint32_t key_in_len,
                        const uint8_t *iv_in, const uint32_t iv_in_len,
                        uint8_t *fixed_input, const uint32_t fixed_input_len,
                        uint8_t *key_out, const uint32_t key_out_len,
                        kbkdf_opts_t *opts)
{

    security_status_e status = SECURITY_STATUS_FAIL;
    kbkdf_opts_t tmp_opts =
        {
            .ctr_rlen = 4,
            .ctr_rpos = 0
        };

    if (!key_in || !key_in_len ||
        !fixed_input || !fixed_input_len ||
        !key_out || !key_out_len ||
        !hmac_callbacks.hmac_final ||
        !hmac_callbacks.hmac_init ||
        !hmac_callbacks.hmac_update)
    {
        return SECURITY_STATUS_FAIL;
    }

    if (opts)
    {
        tmp_opts.ctr_rlen = opts->ctr_rlen;
        tmp_opts.ctr_rpos = opts->ctr_rpos;
    }

    switch (mode)
    {
    case KBKDF_MODE_COUNTER:
        status = _kbkdf_counter(hash_type, hmac_callbacks, key_in, key_in_len, fixed_input,
                                fixed_input_len, key_out, key_out_len, &tmp_opts);
        break;
    case KBKDF_MODE_FEEDBACK:
        status = _kbkdf_feedback(hash_type, hmac_callbacks, key_in, key_in_len, iv_in, iv_in_len,
                                 fixed_input, fixed_input_len, key_out, key_out_len, &tmp_opts);
        break;
    case KBKDF_MODE_DOUBLE_PIPELINE:
        status = _kbkdf_double_pipeline(hash_type, hmac_callbacks, key_in, key_in_len, fixed_input,
                                        fixed_input_len, key_out, key_out_len, &tmp_opts);
        break;
    default:
        status = -1;
    }
    return status;
}