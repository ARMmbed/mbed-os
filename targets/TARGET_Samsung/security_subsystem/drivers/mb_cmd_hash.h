/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef SSS_MB_CMD_HASH_H
#define SSS_MB_CMD_HASH_H

/*************** Include Files ********************************************/
#include "sss_common.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/
#define MAX_HASH_BLOCK_BLEN             (128)   //when sha2

//!mailbox command

#define FUNC_HASH_INIT                  (0x00013101)
#define FUNC_HASH_UPDATE                (0x00023101)
#define FUNC_HASH_FINAL                 (0x00033101)


/*************** New Data Types (Basic Data Types)  ***********************/

//! @struct sHASH_MSG
//! @brief struct of message for Hash
struct sHASH_MSG {

    unsigned int addr_low;

    unsigned int addr_high;

    unsigned int descriptor_byte_len;

    unsigned int msg_byte_len;

    unsigned int msg_type;
};

#define MAX_MB_HASH_BLOCK_BLEN  (256)


/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

unsigned int mb_hash_init(
    const stOCTET_STRING *pstMessage,
    unsigned int object_id);

unsigned int mb_hash_update(
    stOCTET_STRING *pstMessage,
    unsigned int block_byte_len);

unsigned int mb_hash_final(
    stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest);

#endif  /* SSS_MB_CMD_HASH_H */

