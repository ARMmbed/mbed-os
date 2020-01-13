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
#ifndef SSS_DRIVER_RNG_H
#define SSS_DRIVER_RNG_H

/*************** Include Files ********************************************/
#include "sss_common.h"
#include "mb_cmd_rng.h"
#include "mb_cmd_system.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

unsigned int sss_generate_random(stOCTET_STRING *pstRandom, unsigned int request_byte_len);
unsigned int sss_generate_rawrandom(stOCTET_STRING *pstRandom, unsigned int request_byte_len);
unsigned int sss_KAT_RNG(void);

#endif  /* SSS_DRIVER_RNG_H */
