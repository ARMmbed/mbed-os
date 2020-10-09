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
#ifndef MB_CMD_RNG_H
#define MB_CMD_RNG_H

/*************** Include Files ********************************************/
#include "sss_common.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/
// ======================================
// Mailbox Command
// ======================================
#define FUNC_RNG        (0x00016101)
#define FUNC_TRNG       (0x00026101)
#define FUNC_RNG_KAT    (0x00A06101)

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

unsigned int mb_generate_random_number(stOCTET_STRING *pstRandom, unsigned int request_byte_len);
unsigned int mb_generate_raw_random(stOCTET_STRING *pstRandom, unsigned int request_byte_len);
unsigned int mb_KAT_RNG(void);


#endif  /* MB_CMD_RNG_H */

