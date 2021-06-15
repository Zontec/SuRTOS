/****************************INFORMATION***********************************
*
*
*
*
**************************************************************************/

/*!
*   @file       assert.h
*   @brief      assert header file
*	@author     Zontec
*	@version    1.0
*	@date       2021.04.04
*/

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "kernel_config.h"
#include "platform_specific.h"


#ifndef __FILE__
#define __FILE__    "unknown!"
#endif

#ifndef __LINE__
#define __LINE__    (0)
#endif

#if (ASSERT_ENABLED == TRUE) || defined(DOXYGEN)

/*!
*   @brief ASSERT
*   @warning Not tested
*   @param[in] state Used to specify type of Secure Hash Algorithm
*   @param[in] msg Data to be hashed
*   @return void
*/
#define ASSERT(state, msg) assert(state, msg, __FILE__, __LINE__)

/**
	\brief Secured Hash Algortihm
	\warning Not complitly finished

    \param[in] sha_type Used to specify type of Secure Hash Algorithm
    \param[in] message Data to be hashed
    \param[in] message_len Length of data to be hashed
    \param[out] out Hashed data
    \return sha_status_e
*/
void assert(uint8_t state, const char *msg, const char *file, uint32_t line);

#else

#define ASSERT(state, msg)

#endif /* ASSERT_ENABLED */

#endif /* __ASSERT_H__ */