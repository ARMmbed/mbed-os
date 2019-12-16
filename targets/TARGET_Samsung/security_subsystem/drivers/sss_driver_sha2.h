/**************************************************************************/
/* Copyright (c) Samsung Electronics Co., Ltd. All rights reserved.       */
/*                                                                        */
/* -INSTRUCTIONS-                                                         */
/* THIS SOFTWARE IS A CONFIDENTIAL STUFFS AND PROPRIETARY OF SAMSUNG ELEC */
/* TRONICS CO.,LTD. SO YOU SHALL NOT DISCLOSE THIS SOFTWARE OTHER COMPANY */
/* OR PERSONS WITHOUT PERMISSION OF SAMSUNG AND SHALL USE THIS SOFTWARE   */
/* ONLY IN ACCORDANCE WITH THE LICENSE AGREEMENT OF SAMSUNG.              */
/* SAMSUNG MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY   */
/* OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED  */
/* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR */
/* PURPOSE, OR NON-INFRINGEMENT. SAMSUNG SHALL NOT BE LIABLE FOR ANY      */
/* DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR        */
/* DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.                         */
/**************************************************************************/

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

