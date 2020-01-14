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
/* Driver Common */
#include "sss_common.h"
#include "sss_driver_util.h"
#include "sss_map.h"
#include "sss_driver_error.h"
#include "mb_cmd_system.h"

/* Driver Algorithm */
#include "mb_cmd_rng.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

/*! @fn     mb_generate_random_number(unsigned int *random_number, unsigned int random_wlen)
 *  @ingroup    SECURITY_SSS_MAILBOX
 *  @brief      mailbox api for random number generation
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2018.01.13 Modified for S111 SRAM code
 *  @param[out] random_number   : array of random number
 *  @param[in]  random_wlen     : word length of random number
 *  @retval     SUCCESS         : Success
 *  @retval     Others(!=0)     : fail - error from sub-function
 */
unsigned int mb_generate_random_number(stOCTET_STRING *pstRandom, unsigned int request_byte_len)
{
    unsigned int ret;
    unsigned int rsp_byte_len;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }

    //! set data field
    // Check boudary of msg_block
    if (request_byte_len > 256) {
        return ERROR_RNG_INVALID_LEN_REQUEST;
    }

    //! step 1-2 : set control field
    SSS_CTRL_FIELD_SET(1U, request_byte_len);

    //! step 2   : run mb_cmd
    SSS_SET_MB_COMMAND(FUNC_RNG);

    //! step 3   : wait for response
    WAIT_SFR_BIT_CLR(SSS_MB_STATUS, 0x01);

    //! step 4   : get return value// get return value
    SSS_GET_MB_RESULT(ret);
    if (ret == RSP_SUCCESS) {
        SSS_CTRL_FIELD_GET(1U, rsp_byte_len);
        ret = _sss_MB_to_OS(pstRandom, (unsigned int *)(SSS_DATA_FIELD_BASE), rsp_byte_len);
    } else {
        SSS_GET_ERROR_CODE(ret);
    }

    return ret;
}


/*! @fn     mb_generate_random_number(unsigned int *random_number, unsigned int random_wlen)
 *  @ingroup    SECURITY_SSS_MAILBOX
 *  @brief      mailbox api for random number generation
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.51 : 2018.01.13 Modified for S111 SRAM code
 *  @param[out] random_number   : array of random number
 *  @param[in]  random_wlen     : word length of random number
 *  @retval     SUCCESS         : Success
 *  @retval     Others(!=0)     : fail - error from sub-function
 */
unsigned int mb_generate_raw_random(stOCTET_STRING *pstRandom, unsigned int request_byte_len)
{
    unsigned int ret;
    unsigned int rsp_byte_len;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }


    //! set data field
    // Check boudary of msg_block
    if (request_byte_len > 256U) {
        return ERROR_RNG_INVALID_LEN_REQUEST;
    }

    //! step 1-2 : set control field
    SSS_CTRL_FIELD_SET(1U, request_byte_len);

    //! step 2   : run mb_cmd
    SSS_SET_MB_COMMAND(FUNC_TRNG);

    //! step 3   : wait for response
    WAIT_SFR_BIT_CLR(SSS_MB_STATUS, 0x01);

    //! step 4   : get return value// get return value
    SSS_GET_MB_RESULT(ret);
    if (ret == RSP_SUCCESS) {
        SSS_CTRL_FIELD_GET(1U, rsp_byte_len);
        ret = _sss_MB_to_OS(pstRandom, (unsigned int *)(SSS_DATA_FIELD_BASE), rsp_byte_len);
    } else {
        SSS_GET_ERROR_CODE(ret);
    }

    return ret;
}



unsigned int mb_KAT_RNG(void)
{
    unsigned int ret;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }

    //! step 2   : run mb_cmd
    SSS_SET_MB_COMMAND(FUNC_RNG_KAT);

    //! step 3   : wait for response
    WAIT_SFR_BIT_CLR(SSS_MB_STATUS, 0x01);

    //! step 4   : get return value// get return value
    SSS_GET_MB_RESULT(ret);
    if (ret == RSP_SUCCESS) {
        ret = SSSR_SUCCESS;
    } else {
        SSS_GET_ERROR_CODE(ret);
    }

    return ret;
}
