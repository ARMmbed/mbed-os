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

#ifndef SSS_DRIVER_RNG_H
#define SSS_DRIVER_RNG_H

/*************** Include Files ********************************************/
#include "sss_common.h"
#include "mb_cmd_rng.h"
#include "mb_cmd_system.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/

/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/

/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/

unsigned int sss_generate_random(stOCTET_STRING *pstRandom, unsigned int request_byte_len);
unsigned int sss_generate_rawrandom(stOCTET_STRING *pstRandom, unsigned int request_byte_len);
unsigned int sss_KAT_RNG(void);

#endif  /* SSS_DRIVER_RNG_H */
