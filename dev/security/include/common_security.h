
#ifndef __COMMON_SECURITY_H__
#define __COMMON_SECURITY_H__

#include "types.h"

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>


#define SECURITY_SOURCE_UNDEFINED          (0)
#define SECURITY_SOURCE_MBEDTLS            (1)
#define SECURITY_SOURCE_OPENSSL            (2)
#define SECURITY_SOURCE_COMMON             (3)


#ifndef SECURITY_USE_HW_SHA
#define SECURITY_USE_HW_SHA             (FALSE)
#endif /*SECURITY_USE_HW_SHA*/

#ifndef SECURITY_USE_HW_RSA
#define SECURITY_USE_HW_RSA             (FALSE)
#endif /*SECURITY_USE_HW_RSA*/

#ifndef SECURITY_USE_HW_AES
#define SECURITY_USE_HW_AES             (FALSE)
#endif /*SECURITY_USE_HW_AES*/

#ifndef SECURITY_USE_HW_HMAC
#define SECURITY_USE_HW_HMAC            (FALSE)
#endif /*SECURITY_USE_HW_HMAC*/

#ifndef SECURITY_USE_HW_RAND
#define SECURITY_USE_HW_RAND            (FALSE)
#endif /*SECURITY_USE_HW_RAND*/

#ifndef SECURITY_USE_HW_PBKDF
#define SECURITY_USE_HW_PBKDF           (FALSE)
#endif /*SECURITY_USE_HW_PBKDF*/

#ifndef SECURITY_USE_HW_KBKDF
#define SECURITY_USE_HW_KBKDF           (FALSE)
#endif /*SECURITY_USE_HW_KBKDF*/



#if (SECURITY_USE_HW_SHA == FALSE) && !defined(SECURITY_SHA_SOURCE)
#define SECURITY_SHA_SOURCE                     (SECURITY_SOURCE_OPENSSL)
#endif /*SECURITY_SHA_SOURCE*/

#if (SECURITY_USE_HW_RSA == FALSE) && !defined(SECURITY_RSA_SOURCE)
#define SECURITY_RSA_SOURCE                     (SECURITY_SOURCE_OPENSSL)
#endif /*SECURITY_RSA_SOURCE*/

#if (SECURITY_USE_HW_AES == FALSE) && !defined(SECURITY_AES_SOURCE)
#define SECURITY_AES_SOURCE                     (SECURITY_SOURCE_OPENSSL)
#endif /*SECURITY_AES_SOURCE*/

#if (SECURITY_USE_HW_RAND == FALSE) && !defined(SECURITY_RAND_SOURCE)
#define SECURITY_RAND_SOURCE                    (SECURITY_SOURCE_OPENSSL)
#endif /*SECURITY_RAND_SOURCE*/

#if (SECURITY_USE_HW_HMAC == FALSE) && !defined(SECURITY_HMAC_SOURCE)
#define SECURITY_HMAC_SOURCE                    (SECURITY_SOURCE_OPENSSL)
#endif /*SECURITY_HMAC_SOURCE*/

#if (SECURITY_USE_HW_PBKDF == FALSE) && !defined(SECURITY_PBKDF_SOURCE)
#define SECURITY_PBKDF_SOURCE                   (SECURITY_SOURCE_OPENSSL)
#endif /*SECURITY_PBKDF_SOURCE*/

#if (SECURITY_USE_HW_KBKDF == FALSE) && !defined(SECURITY_KBKDF_SOURCE)
#define SECURITY_KBKDF_SOURCE                   (SECURITY_SOURCE_OPENSSL)
#endif /*SECURITY_KBKDF_SOURCE*/


#endif /*__COMMON_SECURITY_H__*/