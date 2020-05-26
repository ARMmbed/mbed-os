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

#ifndef SSS_DRIVER_SHA2_H
#define SSS_DRIVER_SHA2_H

/*************** Include Files ********************************************/
#include "sss_common.h"
#include "mb_cmd_hash.h"
#include "mb_cmd_system.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

/*!
 * @brief       SHA2-256 with normal mode
 * @param[in]   pstMessage      pointer of struct for Message

        Name                        Description                                             Variables
        pu08Data                    array of Data
        u32DataByteLen              Byte length of Data                                 (0< x < 2^32)

 * @param[out]  pstDigest       pointer of struct for Digest

        Name                        Description                                             Variables
        pu08Data                    array of Data
        u32DataByteLen              Byte length of Data                                 32

 * @return

        Error Code                                  Description
        SSSR_SUCCESS                                Function operates normally
        ERROR_SHA_INVALID_VAL_OID_ALG
        ERROR_SHA_INVALID_LEN_MSG

 * @author      kitak Kim (kitak81.kim@samsung.com)
 * @version     V0.00

        Version     Date        Person      Description
        V0.00       2017.01.15  kitak       Initial Version
 */
int sss_SHA2_256(
    const stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest);

/*!
 * @brief       SHA2-384 with normal mode
 * @param[in]   pstMessage      pointer of struct for Message

        Name                        Description                                             Variables
        pu08Data                    array of Data
        u32DataByteLen              Byte length of Data                                 (0< x < 2^32)

 * @param[out]  pstDigest       pointer of struct for Digest

        Name                        Description                                             Variables
        pu08Data                    array of Data
        u32DataByteLen              Byte length of Data                                 48

 * @return

        Error Code                                  Description
        SSSR_SUCCESS                                Function operates normally
        ERROR_SHA_INVALID_VAL_OID_ALG
        ERROR_SHA_INVALID_LEN_MSG

 * @author      kitak Kim (kitak81.kim@samsung.com)
 * @version     V0.00

        Version     Date        Person      Description
        V0.00       2017.01.15  kitak       Initial Version
        V0.01       2018.01.09  kitak       Modify for S111 SRAM code
 */
int sss_SHA2_384(
    const stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest);

/*!
 * @brief       SHA2-512 with normal mode
 * @param[in]   pstMessage      pointer of struct for Message

        Name                        Description                                             Variables
        pu08Data                    array of Data
        u32DataByteLen              Byte length of Data                                 (0< x < 2^32)

 * @param[out]  pstDigest       pointer of struct for Digest

        Name                        Description                                             Variables
        pu08Data                    array of Data
        u32DataByteLen              Byte length of Data                                 64

 * @return

        Error Code                                  Description
        SSSR_SUCCESS                                Function operates normally
        ERROR_SHA_INVALID_VAL_OID_ALG
        ERROR_SHA_INVALID_LEN_MSG

 * @author      kitak Kim (kitak81.kim@samsung.com)
 * @version     V0.00

        Version     Date        Person      Description
        V0.00       2017.01.15  kitak       Initial Version
        V0.01       2018.01.09  kitak       Modify for S111 SRAM code
 */
int sss_SHA2_512(
    const stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest);

#endif  /* ISP_DRIVER_SHA2_H */

