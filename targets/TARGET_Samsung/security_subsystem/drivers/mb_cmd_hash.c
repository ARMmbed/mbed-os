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

/*************** Include Files ********************************************/
/* Driver Common */
#include "sss_common.h"
#include "sss_driver_util.h"
#include "sss_map.h"
#include "sss_driver_error.h"
#include "mb_cmd_system.h"

/* Driver Algorithm */
#include "mb_cmd_hash.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/
const unsigned char pu08hash_wlen[8] = { 0, 5, 7, 8, 12, 16, 0, 0 };

/*************** Functions  ***********************************************/


/*! @fn     mb_hash_init(unsigned int object_id, unsigned int msg_byte_len)
 * @ingroup SECURITY_SSS_MAILBOX
 * @brief       mailbox api for hash to init. hash setting
 * @version    v0.50 : 2016.8.13 Init. release version
 * @param[in]  object_id : algorithm selection
 * @param[in]  msg_byte_len  : length of message with byte unit
 * @retval      SUCCESS             : Success
 * @retval      Others(!=0)             : fail - error from sub-function
 */
unsigned int mb_hash_init(
    const stOCTET_STRING *pstMessage,
    unsigned int object_id)
{

    unsigned int ret;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }

    //! step 1-1 : set data field
    SSS_DATA_FIELD_SET(0U, pstMessage->u32DataByteLen);

    //! step 1-2 : set control field
    SSS_SET_MB_OID(object_id);

    //! step 2   : run mb_cmd
    ret = sss_mb_command(FUNC_HASH_INIT);

    return ret;
}



/*! @fn     mb_hash_update(unsigned int block_byte_len, unsigned char * msg_block)
 *  @ingroup    SECURITY_SSS_MAILBOX
 *  @brief      mailbox api for hash to update partial block of message
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[in]  block_byte_len: length of partial block message. It differ from hash algorithm spec.
 *  @param[in]  msg_block : partial message block with block_byte_len
 *  @retval     SUCCESS             : Success
 *  @retval     Others(!=0)             : fail - error from sub-function
 */
unsigned int mb_hash_update(
    stOCTET_STRING *pstMessage,
    unsigned int block_byte_len)
{
    int ret = SSSR_SUCCESS;

    //! step 1 : digest
    while (pstMessage->u32DataByteLen > MAX_MB_HASH_BLOCK_BLEN) {

        //! - check the status of mailbox
        if (SSS_MB_STATUS) {
            return ERROR_SYSTEM_MAILBOX_BUSY;
        }

        //! Set data field : message
        ret = _sss_OS_to_MB(pstMessage, (unsigned int *)(SSS_DATA_FIELD_BASE), MAX_MB_HASH_BLOCK_BLEN);

        if (ret != SSSR_SUCCESS) {
            return ret;
        }

        //! Run mb_cmd
        ret = sss_mb_command(FUNC_HASH_UPDATE);

        pstMessage->pu08Data += MAX_MB_HASH_BLOCK_BLEN;
        pstMessage->u32DataByteLen -= MAX_MB_HASH_BLOCK_BLEN;

    }

    return ret;
}



/*!
 *  @ingroup    SECURITY_SSS_MAILBOX
 *  @brief      mailbox api for hash to update partial block of message
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @param[out] hash      : hash output
 *  @param[in]  block_byte_len: length of last block message.
 *  @param[in]  msg_block : last message block with block_byte_len
 *  @retval     SUCCESS             : Success
 *  @retval     Others(!=0)             : fail - error from sub-function
 */
unsigned int mb_hash_final(
    stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest)
{
    unsigned int ret;

    //! step 0 : check the status of mailbox
    if (SSS_MB_STATUS) {
        return ERROR_SYSTEM_MAILBOX_BUSY;
    }

    //! set data field
    // Check boudary of msg_block
    if (pstMessage->u32DataByteLen > MAX_MB_HASH_BLOCK_BLEN) {
        return ERROR_HASH_INVALID_LEN_BLOCK;
    }

    ret = _sss_OS_to_MB(pstMessage, (unsigned int *)(SSS_DATA_FIELD_BASE), pstMessage->u32DataByteLen);
    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 2   : run mb_cmd
    ret = sss_mb_command(FUNC_HASH_FINAL);
    if (ret == SSSR_SUCCESS) {
        ret = sss_mb_get_response(pstDigest);
    }

    return ret;
}


