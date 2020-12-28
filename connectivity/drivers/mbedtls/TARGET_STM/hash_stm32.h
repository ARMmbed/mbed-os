/**
  ******************************************************************************
  * @brief   Header file of mbed TLS HW crypto (HASH) implementation.
  ******************************************************************************
  * @attention
  *
  *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
  * SPDX-License-Identifier: Apache-2.0
  *  Copyright (C) 2019 STMicroelectronics, All Rights Reserved
  *
  * This software component is licensed by ST under Apache 2.0 license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  * https://opensource.org/licenses/Apache-2.0
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HASH_H
#define __HASH_H

#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_MD5_ALT)

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cmsis.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

/* macros --------------------------------------------------------------------*/
/* constants -----------------------------------------------------------------*/
#define ST_HASH_TIMEOUT ((uint32_t) 1000)  /* TO in ms for the hash processor */

/* defines -------------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
#if defined(MBEDTLS_THREADING_C)
extern mbedtls_threading_mutex_t hash_mutex;
extern unsigned char hash_mutex_started;
#endif /* MBEDTLS_THREADING_C */

extern unsigned int hash_context_count;

/* functions prototypes ------------------------------------------------------*/
extern void hash_zeroize(void *v, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_SHA1_ALT or MBEDTLS_SHA256_ALT or MBEDTLS_MD5_ALT */
#endif /*__HASH_H */
