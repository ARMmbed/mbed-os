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
#include "sss_driver_error.h"
#include "sss_driver_sha2.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/


int sss_SHA2_256(
    const stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest)
{

    int ret = FAIL;

    stOCTET_STRING stHASH_Input;
    unsigned int object_id;
    unsigned int block_byte_len;

    //! step 0 : clear Mailbox
    ret = mb_system_clear(CLEAR_TYPE_MAILBOX);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 0 : Check validity of parameter "object_id"
#if 0
    if (pstMessage->u32DataByteLen == 0) {
        return ERROR_HASH_INVALID_LEN_MSG;
    }
#endif

    //! assign hash_byte_len to compare returned result from sss_fw after hash operation
    object_id = OID_SHA2_256;
    block_byte_len = 64;

    //! step 1 : set message length parameter to SSS
    ret = mb_hash_init(pstMessage, object_id);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 2 : set message block to SSS
    stHASH_Input.pu08Data = pstMessage->pu08Data;
    stHASH_Input.u32DataByteLen = pstMessage->u32DataByteLen;

    ret = mb_hash_update(&stHASH_Input, block_byte_len);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 3 : get hash result from SSS
    ret = mb_hash_final(&stHASH_Input, pstDigest);

    return ret;
}



int sss_SHA2_384(
    const stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest)
{

    int ret = FAIL;

    stOCTET_STRING stHASH_Input;
    unsigned int object_id;
    unsigned int block_byte_len;

    //! step 0 : clear Mailbox
    ret = mb_system_clear(CLEAR_TYPE_MAILBOX);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 0 : Check validity of parameter "object_id"
#if 0
    if (pstMessage->u32DataByteLen == 0) {
        return ERROR_HASH_INVALID_LEN_MSG;
    }
#endif

    //! assign hash_byte_len to compare returned result from sss_fw after hash operation
    object_id = OID_SHA2_384;
    block_byte_len = 128;

    //! step 1 : set message length parameter to SSS
    ret = mb_hash_init(pstMessage, object_id);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 2 : set message block to SSS
    stHASH_Input.pu08Data = pstMessage->pu08Data;
    stHASH_Input.u32DataByteLen = pstMessage->u32DataByteLen;

    ret = mb_hash_update(&stHASH_Input, block_byte_len);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 3 : get hash result from SSS
    ret = mb_hash_final(&stHASH_Input, pstDigest);

    return ret;
}



int sss_SHA2_512(
    const stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest)
{

    int ret = FAIL;

    stOCTET_STRING stHASH_Input;
    unsigned int object_id;
    unsigned int block_byte_len;

    //! step 0 : clear Mailbox
    ret = mb_system_clear(CLEAR_TYPE_MAILBOX);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 0 : Check validity of parameter "object_id"
#if 0
    if (pstMessage->u32DataByteLen == 0) {
        return ERROR_HASH_INVALID_LEN_MSG;
    }
#endif

    //! assign hash_byte_len to compare returned result from sss_fw after hash operation
    object_id = OID_SHA2_512;
    block_byte_len = 128;

    //! step 1 : set message length parameter to SSS
    ret = mb_hash_init(pstMessage, object_id);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 2 : set message block to SSS
    stHASH_Input.pu08Data = pstMessage->pu08Data;
    stHASH_Input.u32DataByteLen = pstMessage->u32DataByteLen;

    ret = mb_hash_update(&stHASH_Input, block_byte_len);

    if (ret != SSSR_SUCCESS) {
        return ret;
    }

    //! step 3 : get hash result from SSS
    ret = mb_hash_final(&stHASH_Input, pstDigest);

    return ret;
}


