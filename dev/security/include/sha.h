/*!
    \file
    \brief SHA algorithm header file
	\author Zontec
	\version 1.0
	\date 2021.04.04

    File specifises interface for using secured hash algorithm
*/

#ifndef __SHA_H__
#define __SHA_H__

#include "common_security.h"

#if (SECURITY_USE_HW_SHA == TRUE)
#define SHA     hw_sha
#else
#define SHA     fw_sha
#endif /*(SECURITY_USE_HW_SHA == 1)*/

typedef enum sha_type_e {
    SHA_1 = 0,
    SHA_256 = 1,
    SHA_384 = 2,
    SHA_512 = 3,
} sha_type_e;

typedef enum sha_status_e {
    SHA_OK = 0,
    SHA_FAIL = 1,
    SHA_UNKNOWN_HASH_TYPE = 2,
} sha_status_e;

typedef struct sha_t
{
    uint8_t *out;
    uint32_t _h0;
    uint32_t _h1;
    uint32_t _h2;
    uint32_t _h3;
    uint32_t _h4;
};

/**
	\brief Secured Hash Algortihm
	\warning Not complitly finished

    \param[in] sha_type Used to specify type of Secure Hash Algorithm
    \param[in] message Data to be hashed
    \param[in] message_len Length of data to be hashed
    \param[out] out Hashed data
    \return sha_status_e
**/
uint32_t SHA(sha_type_e sha_type, const uint8_t *message, 
            const uint32_t message_len, uint8_t *out);

/**
	\brief Secured Hash Algortihm
	\warning Not complitly finished

    \param[in] sha_type Used to specify type of Secure Hash Algorithm
    \param[in] message Data to be hashed
    \param[in] message_len Length of data to be hashed
    \param[out] out Hashed data
    \return sha_status_e
**/
uint32_t get_sha_hash_size(sha_type_e sha_type);

#endif /*__SHA_H__*/