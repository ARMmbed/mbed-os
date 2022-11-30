/**
 *******************************************************************************
 * @file    txz_flash.c
 * @brief   This file provides API functions for driver common part.
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
  * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_flash.h"

#if defined(__FLASH_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/**
 *  @addtogroup FLASH
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Private_macro FLASH Private Macro
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FLASH_Private_macro */


/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Private_define FLASH Private Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FLASH_Private_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Private_define FLASH Private Define
 *  @{
 */
/**
 *  @defgroup wait Configuration
 *  @brief    wait Configuration.
 *  @{
 */
/* FC ACCR setting  configration */
#define FC_KCR_KEYCODE      (0xA74A9D23UL)              /*!< The specific code that writes the FCKCR register. */
#define FC_ACCR_CODE_7CLK   (0x00000006UL)              /*!< 7clock(less than 200MHZ).                         */
#define FC_ACCR_CODE_6CLK   (0x00000005UL)              /*!< 6clock(less than 160MHZ).                         */
#define FC_ACCR_CODE_5CLK   (0x00000004UL)              /*!< 5clock(less than 120MHZ).                         */
#define FC_ACCR_CODE_4CLK   (0x00000003UL)              /*!< 4clock(less than 100MHZ).                         */
#define FC_ACCR_CODE_3CLK   (0x00000002UL)              /*!< 3clock(less than  80MHZ).                         */
#define FC_ACCR_200MHZ      (200000000UL)               /*!< 200MHZ                                            */
#define FC_ACCR_160MHZ      (160000000UL)               /*!< 160MHZ                                            */
#define FC_ACCR_120MHZ      (120000000UL)               /*!< 120MHZ                                            */
#define FC_ACCR_100MHZ      (100000000UL)               /*!< 100MHZ                                            */
#define FC_ACCR_80MHZ       ( 80000000UL)               /*!<  80MHZ                                            */

#define FC_ACCR_DATA        (FC_ACCR_DATA_7CLK)
#define FC_ACCR_DATA_7CLK   (0x00000600UL)              /*!< 7clock(Must).                                     */

/**
 *  @}
 */ /* End of group waitConfiguration */

/* no define */

/**
 *  @}
 */ /* End of group FLASH_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Private_define FLASH Private Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FLASH_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Private_typedef FLASH Private Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FLASH_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Private_fuctions FLASH Private Fuctions
 *  @{
 */

/**
 *  @}
 */ /* End of group FLASH_Private_functions */

/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FLASH_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief  Raed clock set.
  * @param  none.
  * @retval none.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
void fc_read_clock_set(uint32_t sysclock)
{
    uint32_t regval = 0;

    if (sysclock <= FC_ACCR_80MHZ) {   /* less than 80MHZ           */
        regval = (uint32_t)(FC_ACCR_CODE_3CLK | FC_ACCR_DATA);
    } else if (sysclock <= FC_ACCR_100MHZ) { /* less than 100MHZ         */
        regval = (uint32_t)(FC_ACCR_CODE_4CLK | FC_ACCR_DATA);
    } else if (sysclock <= FC_ACCR_120MHZ) { /* less than 120MHZ         */
        regval = (uint32_t)(FC_ACCR_CODE_5CLK | FC_ACCR_DATA);
    } else if (sysclock <= FC_ACCR_160MHZ) { /* less than 160MHZ         */
        regval = (uint32_t)(FC_ACCR_CODE_6CLK | FC_ACCR_DATA);
    } else if (sysclock <= FC_ACCR_200MHZ) { /* less than 200MHZ         */
        regval = (uint32_t)(FC_ACCR_CODE_7CLK | FC_ACCR_DATA);
    }

    TSB_FC->KCR = FC_KCR_KEYCODE;
    TSB_FC->ACCR = regval;
    while ((TSB_FC->ACCR != (uint32_t)(FC_ACCR_CODE_3CLK | FC_ACCR_DATA))
            && (TSB_FC->ACCR != (uint32_t)(FC_ACCR_CODE_4CLK | FC_ACCR_DATA))
            && (TSB_FC->ACCR != (uint32_t)(FC_ACCR_CODE_5CLK | FC_ACCR_DATA))
            && (TSB_FC->ACCR != (uint32_t)(FC_ACCR_CODE_6CLK | FC_ACCR_DATA))
            && (TSB_FC->ACCR != (uint32_t)(FC_ACCR_CODE_7CLK | FC_ACCR_DATA))) {
        /* no processing */
    }
}

/**
 *  @}
 */ /* End of group FLASH_Exported_functions */

/**
 *  @}
 */ /* End of group FLASH */

/**
 *  @}
 */ /* End of group Periph_Driver */

#endif /* defined(__FLASH_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

