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
/*************** Include Files ********************************************/
#include "sss_driver_error.h"
#include "sss_driver_rng.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

/*! @fn     sss_generate_random(unsigned int *random, unsigned int wlen)
 *  @ingroup    SECURITY_SSS
 *  @brief      rng function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.01 : 2018.01.13 Modified for S111 SRAM code
 *  @param[out] random             : array of random number
 *  @param[in]  wlen               : word length of random number to be generated
 *  @retval     SUCCESS            : Success
 *  @retval     Others(!=0)        : fail - error from sub-function
 */
unsigned int sss_generate_random(stOCTET_STRING *pstRandom, unsigned int request_byte_len)
{

    unsigned int ret;

    //! step 0 : clear Mailbox
    ret = mb_system_clear(CLEAR_TYPE_MAILBOX);
    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 1 : generate random
    ret = mb_generate_random_number(pstRandom, request_byte_len);

    return ret;
}


/*!
 *  @ingroup    SECURITY_SSS
 *  @brief      rng function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2018.01.13 Modified for S111 SRAM code
 *  @param[out] random             : array of random number
 *  @param[in]  wlen               : word length of random number to be generated
 *  @retval     SUCCESS            : Success
 *  @retval     Others(!=0)        : fail - error from sub-function
 */
unsigned int sss_generate_rawrandom(stOCTET_STRING *pstRandom, unsigned int request_byte_len)
{

    unsigned int ret;

    //! step 0 : clear Mailbox
    ret = mb_system_clear(CLEAR_TYPE_MAILBOX);
    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 1 : generate random
    ret = mb_generate_raw_random(pstRandom, request_byte_len);

    return ret;
}



/*!
 *  @ingroup    SECURITY_SSS
 *  @brief      rng function
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2018.01.13 Modified for S111 SRAM code
 *  @param[out] random             : array of random number
 *  @param[in]  wlen               : word length of random number to be generated
 *  @retval     SUCCESS            : Success
 *  @retval     Others(!=0)        : fail - error from sub-function
 */
unsigned int sss_KAT_RNG(void)
{

    unsigned int ret;

    //! step 0 : clear Mailbox
    ret = mb_system_clear(CLEAR_TYPE_MAILBOX);
    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 1 : call KAT
    ret = mb_KAT_RNG();

    return ret;
}


