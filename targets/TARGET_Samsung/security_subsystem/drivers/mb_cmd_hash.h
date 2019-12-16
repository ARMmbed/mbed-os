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

#ifndef SSS_MB_CMD_HASH_H
#define SSS_MB_CMD_HASH_H

/*************** Include Files ********************************************/
#include "sss_common.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/
#define MAX_HASH_BLOCK_BLEN             (128)   //when sha2

//!mailbox command

#define FUNC_HASH_INIT                  (0x00013101)
#define FUNC_HASH_UPDATE                (0x00023101)
#define FUNC_HASH_FINAL                 (0x00033101)


/*************** New Data Types (Basic Data Types)  ***********************/

//! @struct sHASH_MSG
//! @brief struct of message for Hash
struct sHASH_MSG {

    unsigned int addr_low;

    unsigned int addr_high;

    unsigned int descriptor_byte_len;

    unsigned int msg_byte_len;

    unsigned int msg_type;
};

#define MAX_MB_HASH_BLOCK_BLEN  (256)


/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

unsigned int mb_hash_init(
    const stOCTET_STRING *pstMessage,
    unsigned int object_id);

unsigned int mb_hash_update(
    stOCTET_STRING *pstMessage,
    unsigned int block_byte_len);

unsigned int mb_hash_final(
    stOCTET_STRING *pstMessage,
    stOCTET_STRING *pstDigest);

#endif  /* SSS_MB_CMD_HASH_H */

