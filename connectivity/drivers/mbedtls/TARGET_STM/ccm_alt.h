/**
 * \file ccm.h
 *
 * \brief This file provides an API for the CCM authenticated encryption
 *        mode for block ciphers.
 *
 * CCM combines Counter mode encryption with CBC-MAC authentication
 * for 128-bit block ciphers.
 *
 * Input to CCM includes the following elements:
 * <ul><li>Payload - data that is both authenticated and encrypted.</li>
 * <li>Associated data (Adata) - data that is authenticated but not
 * encrypted, For example, a header.</li>
 * <li>Nonce - A unique value that is assigned to the payload and the
 * associated data.</li></ul>
 *
 * Definition of CCM:
 * http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf
 * RFC 3610 "Counter with CBC-MAC (CCM)"
 *
 * Related:
 * RFC 5116 "An Interface and Algorithms for Authenticated Encryption"
 *
 * Definition of CCM*:
 * IEEE 802.15.4 - IEEE Standard for Local and metropolitan area networks
 * Integer representation is fixed most-significant-octet-first order and
 * the representation of octets is most-significant-bit-first order. This is
 * consistent with RFC 3610.
 */
/*
 *  Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved
 *  Copyright (C) 2019-2020 STMicroelectronics, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file implements ST CCM HW services based on API from mbed TLS
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MBEDTLS_CCM_ALT_H
#define MBEDTLS_CCM_ALT_H

#if defined(MBEDTLS_CCM_ALT)
/* Includes ------------------------------------------------------------------*/
#include "cryp_stm32.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/**
 * \brief    The CCM context-type definition. The CCM context is passed
 *           to the APIs called.
 */
typedef struct mbedtls_ccm_context {
    /* Encryption/Decryption key */
    uint32_t ccm_key[8];

    CRYP_HandleTypeDef hcryp_ccm;         /* CCM context                     */
    uint32_t           ctx_save_cr;       /* save context for multi-context  */
}
mbedtls_ccm_context;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_CCM_ALT */
#endif /* MBEDTLS_CCM_ALT_H */
