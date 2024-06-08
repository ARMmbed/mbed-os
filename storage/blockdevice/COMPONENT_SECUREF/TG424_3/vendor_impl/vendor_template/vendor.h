/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _VENDOR_H_
#define _VENDOR_H_

#include "../../../JEDEC_security_HAL/vendor_security_impl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Define your macros here
 *
 */
#define VENOR_ID 0xFF

/**
 * Secure Flash supported security operations definition
 *
 */
typedef enum {
    SECURITY_READ,              /*!< Security read */
    SECURITY_WRITE,             /*!< Security program */
    SECURITY_ERASE,             /*!< Security erase */
    MC_INCREASEMENT,            /*!< Increase monotonic counter */
    MC_READ,                    /*!< Read monotonic counter */
    GENERATE_NONCE,             /*!< Generate random number */
    CREATE_SESSION,             /*!< Session creation */
    CONFIRM_SESSION,            /*!< Session confirmation */
    TERMINATE_SESSION,          /*!< Session termination */
    GEN_ROOT_KEY,               /*!< Generate root key */
    SECURITY_GET_REGION_CONFIG, /*!< Get region configure in security */
    CFG_SECURITY_PROFILE,       /*!< Set secure Flash security profile */
    SECURE_INIT,                /*!< Initialize in security */
    SECURE_UNINIT,              /*!< Unitialize in security */
    LOCK_DOWN                   /*!< Lock down */
    /* Add secure Flash operations here */
    //TODO
} vendor_secureflash_operation_type_t;

/**
 * \struct vendor_secureflash_security_operation_t
 *
 * \brief The structure holding secure Flash security operations' context.
 */
typedef struct {
//TODO
} vendor_secureflash_security_operation_t;


/**
 * \struct secureflash_registers_t
 *
 * \brief The structure holding secure Flash registers' value.
 */
typedef struct {
//TODO
} secureflash_registers_t;


typedef struct {
    const char *name;
    //TODO
    /* Add vendor context members here */
} vendor_secureflash_context;

extern vendor_security_op_t vendor_secureflash;

#ifdef __cplusplus
}
#endif

#endif /* _VENDOR_H_ */
