/**
  ******************************************************************************
  * @brief   Header file of mbed TLS HW crypto (CRYP) implementation.
  ******************************************************************************
  * @attention
  *
  *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  * SPDX-License-Identifier: Apache-2.0
  *  Copyright (C) 2019-2020 STMicroelectronics, All Rights Reserved
  *
  * This software component is licensed by ST under Apache 2.0 license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  * https://opensource.org/licenses/Apache-2.0
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRYP_H
#define __CRYP_H

#if !(TARGET_STM32L4)
#if defined(MBEDTLS_AES_ALT) || defined(MBEDTLS_CCM_ALT) || defined(MBEDTLS_GCM_ALT)

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cmsis.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

/* macros --------------------------------------------------------------------*/
/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i)                            \
do {                                                    \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
        | ( (uint32_t) (b)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 3]       );            \
} while( 0 )
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
do {                                                    \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
} while( 0 )
#endif

/* constants -----------------------------------------------------------------*/
#define ST_CRYP_TIMEOUT   1000  /* timeout (in ms) for the crypto processor   */

/* defines -------------------------------------------------------------------*/
/* AES 192 bits key length may be optional in the HW */
#if defined CRYP_KEYSIZE_192B
#define USE_AES_KEY192          1
#else
#define USE_AES_KEY192          0
#endif /* USE_AES_KEY192 */

/* variables -----------------------------------------------------------------*/
#if defined(MBEDTLS_THREADING_C)
extern mbedtls_threading_mutex_t cryp_mutex;
extern unsigned char cryp_mutex_started;
#endif /* MBEDTLS_THREADING_C */

extern unsigned int cryp_context_count;

/* functions prototypes ------------------------------------------------------*/
extern void cryp_zeroize(void *v, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_AES_ALT or MBEDTLS_CCM_ALT or MBEDTLS_GCM_ALT */
#endif /* ! TARGET_STM32L4 */
#endif /*__CRYP_H */
