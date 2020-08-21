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
#include "sss_map.h"
#include "sss_driver_error.h"
#include "sss_driver_util.h"
#include "mb_cmd_system.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

unsigned int mb_system_clear(unsigned int type)
{
    int ret;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }
    //! set data field

    //! step 1-2 : set control field
    SSS_CTRL_FIELD_SET(1U, type);

    //! step 2   : run mb_cmd
    SSS_SET_MB_COMMAND(FUNC_SYSTEM_CLEAR);

    //! step 3   : wait for response
    WAIT_SFR_BIT_CLR(SSS_MB_STATUS, 0x01);

    //! step 4   : get return value// get return value
    SSS_GET_MB_RESULT(ret);

    if (ret == RSP_SUCCESS) {
        ret = SUCCESS;
    } else {
        SSS_GET_ERROR_CODE(ret);
    }

    return ret;
}


unsigned int mb_system_get_info(unsigned int *version)
{

    int ret;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }
    //! set data field

    //! step 1-2 : set control field

    //! step 2   : run mb_cmd
    SSS_SET_MB_COMMAND(FUNC_SYSTEM_GET_INFO);

    //! step 3   : wait for response
    WAIT_SFR_BIT_CLR(SSS_MB_STATUS, 0x03);

    //! step 4   : get return value// get return value
    SSS_GET_MB_RESULT(ret);

    if (ret == RSP_SUCCESS) {
        //_isp_memcpy_mailbox((unsigned int *)version , (unsigned int *)(DATA_FIELD_BASE), 8);
        *version++ = SSS_DATA_FIELD(0);
        *version++ = SSS_DATA_FIELD(1);
        ret = SUCCESS;
    } else {
        SSS_GET_ERROR_CODE(ret);
    }

    return ret;
}


unsigned int mb_system_get_info_sram(unsigned int *version)
{

    int ret;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }
    //! set data field

    //! step 1-2 : set control field

    //! step 2   : run mb_cmd
    SSS_SET_MB_COMMAND(0x00000102);

    //! step 3   : wait for response
    WAIT_SFR_BIT_CLR(SSS_MB_STATUS, 0x03);

    //! step 4   : get return value// get return value
    SSS_GET_MB_RESULT(ret);

    if (ret == RSP_SUCCESS) {
        //_isp_memcpy_mailbox((unsigned int *)version , (unsigned int *)(DATA_FIELD_BASE), 8);
        *version++ = SSS_DATA_FIELD(0);
        *version++ = SSS_DATA_FIELD(1);
        ret = SUCCESS;
    } else {
        SSS_GET_ERROR_CODE(ret);
    }

    return ret;
}


unsigned int mb_system_fw_loading(unsigned int u32REMAP_ADDR, unsigned int u32SFR_REMAP_ADDR)
{

    int ret;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }
    //! set data field
    SSS_DATA_FIELD_SET(0, u32REMAP_ADDR);

    SSS_DATA_FIELD_SET(1, u32SFR_REMAP_ADDR);

    //! step 1-2 : set control field

    //! step 2   : run mb_cmd
    SSS_SET_MB_COMMAND(FUNC_SYSTEM_SELF_LOAD);

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


unsigned int sss_mb_command(unsigned int u32Command)
{
    unsigned int ret;

    //! - Run mb_cmd
    SSS_SET_MB_COMMAND(u32Command);

    //! - Wait for response
    WAIT_SFR_BIT_CLR(SSS_MB_STATUS, 0x01);

    //! - Check result
    ret = sss_mb_check_error();
    return ret;
}


unsigned int sss_mb_check_n_get_response(stOCTET_STRING *pstOutput, unsigned int u32ExpecedLen)
{
    unsigned int ret;
    unsigned int rsp_block_byte_len;

    SSS_GET_RSP_LEN(rsp_block_byte_len);

    if (rsp_block_byte_len == u32ExpecedLen) {

        ret = _sss_MB_to_OS(pstOutput, (unsigned int *)(SSS_DATA_FIELD_BASE), rsp_block_byte_len);
    } else {
        return ERROR_INVALID_LEN_RSP_BLOCK;
    }

    return ret;
}


unsigned int sss_mb_get_response(stOCTET_STRING *pstOutput)
{
    unsigned int ret;
    unsigned int rsp_block_byte_len;

    SSS_GET_RSP_LEN(rsp_block_byte_len);
    ret = _sss_MB_to_OS(pstOutput, (unsigned int *)(SSS_DATA_FIELD_BASE), rsp_block_byte_len);

    return ret;
}


unsigned int sss_mb_check_n_get_response_BN(stBIG_NUM *pstOutput, unsigned int u32ExpecedLen)
{
    unsigned int ret;
    unsigned int rsp_block_byte_len;

    SSS_GET_RSP_LEN(rsp_block_byte_len);

    if (rsp_block_byte_len == u32ExpecedLen) {
        ret = _sss_MB_to_BN(pstOutput, (unsigned int *)(SSS_DATA_FIELD_BASE), rsp_block_byte_len);
    } else {
        return ERROR_INVALID_LEN_RSP_BLOCK;
    }

    return ret;
}


unsigned int sss_mb_get_response_BN(stBIG_NUM *pstOutput)
{
    unsigned int ret;
    unsigned int rsp_block_byte_len;

    SSS_GET_RSP_LEN(rsp_block_byte_len);
    ret = _sss_MB_to_BN(pstOutput, (unsigned int *)(SSS_DATA_FIELD_BASE), rsp_block_byte_len);

    return ret;
}


unsigned int sss_mb_check_error(void)
{
    unsigned int ret;

    SSS_GET_MB_RESULT(ret);

    if (ret == RSP_SUCCESS) {
        ret = SSSR_SUCCESS;
    } else {
        SSS_GET_ERROR_CODE(ret);
    }
    return ret;
}

