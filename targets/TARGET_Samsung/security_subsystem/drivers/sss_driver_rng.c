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
 *  @version    v0.01 : 2018.01.13 Modifyh for S111 SRAM code
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
 *  @version    v0.51 : 2018.01.13 Modifyh for S111 SRAM code
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
 *  @version    v0.51 : 2018.01.13 Modifyh for S111 SRAM code
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


