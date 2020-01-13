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
#ifndef MB_CMD_SYSTEM_H
#define MB_CMD_SYSTEM_H

/*************** Include Files ********************************************/
#include "sss_common.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/
#define CLEAR_TYPE_MAILBOX                  (0x00)
#define CLEAR_TYPE_ALL                      (0xff)

#define ERROR_INVALID_LEN_RSP_BLOCK         (FROM_DRV | (INVALID_LEN | ERR_OUTPUT))

//! Mailbox Command
#define FUNC_SYSTEM_GET_INFO                (0x00000101)
#define FUNC_SYSTEM_CLEAR                   (0x00000201)
#define FUNC_SYSTEM_SELF_LOAD               (0x00000501)


/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

/*!
 * @brief       mailbox command for clear function
 * @param[in]   type                definition of clear level                               {}

 * @return

        Error Code                                              Description
        SSSR_SUCCESS                                            Function operates normally
        Others(!=0)                                             fail - error from sub-function

 * @author      Jinsu Hyun
 * @version     V0.5

        Version     Date        Person      Description
        V0.50       2016.08.13  jinsu       Initial Version
        V0.51       2018.01.04  kitak       Modify for S111 SRAM code

 */
unsigned int mb_system_clear(unsigned int type);

/*!
 * @brief       mailbox command for get info function
 * @param[in]   version             ???                                                     ???

 * @return

        Error Code                                              Description
        SSSR_SUCCESS                                            Function operates normally
        Others(!=0)                                             fail - error from sub-function

 * @author      Jinsu Hyun
 * @version     V0.5

        Version     Date        Person      Description
        V0.50       2016.08.13  jinsu       Initial Version
        V0.51       2018.01.04  kitak       Modify for S111 SRAM code

 */
unsigned int mb_system_get_info(unsigned int *version);

unsigned int mb_system_get_info_sram(unsigned int *version);


/**
 * @brief       mailbox command for self-loading
 * @param[in]   u32SrcAddr          Address of FW to be loaded
 * @return

        Error Code                                  Description
        SSSR_SUCCESS                                Function operates normally
        ERROR_IP_BUSY
        ERROR_INVALID_OID_SIZE

 * @author      kiseok.bae (kiseok.bae@samsung.com)
 * @version     V0.00

        Version     Date        Person      Description
        V0.00       2017.07.15  kiseok      Initial Version
        V0.51       2018.01.04  kitak       Modify for S111 SRAM code
 */
unsigned int mb_system_fw_loading(unsigned int u32REMAP_ADDR, unsigned int u32SFR_REMAP_ADDR);


unsigned int sss_mb_command(unsigned int u32Command);
unsigned int sss_mb_check_error(void);
unsigned int sss_mb_check_n_get_response(stOCTET_STRING *pstOutput, unsigned int u32ExpecedLen);
unsigned int sss_mb_get_response(stOCTET_STRING *pstOutput);
unsigned int sss_mb_get_response_BN(stBIG_NUM *pstOutput);
unsigned int sss_mb_check_n_get_response_BN(stOCTET_STRING *pstOutput, unsigned int u32ExpecedLen);

#endif /* MB_CMD_SYSTEM_H */

