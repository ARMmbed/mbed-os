/**
 *******************************************************************************
 * @file    txz_hal.c
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
#include "txz_hal.h"

#if defined(__HAL_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/**
 *  @addtogroup HAL
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Private_macro HAL Private Macro
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Private_macro */


/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Private_define HAL Private Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Private_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Private_define HAL Private Define
 *  @{
 */
/**
 *  @defgroup wait Configuration
 *  @brief    wait Configuration.
 *  @{
 */
#define WAIT_UNIT_1US          ((uint32_t)(1000000))     /*!< 1S-1us transfor unit.   */
#define WAIT_FORLOOP_STEPS           ((uint32_t)(5))     /*!< for loop steps.         */
#define FC_KCR_KEYCODE      (0xA74A9D23UL)              /*!< The specific code that writes the FCKCR register. */
#define FC_ACCR_200MHZ      (200000000UL)               /*!< 200MHZ                                            */
#define FC_ACCR_160MHZ      (160000000UL)               /*!< 160MHZ                                            */
#define FC_ACCR_120MHZ      (120000000UL)               /*!< 120MHZ                                            */
#define FC_ACCR_100MHZ      (100000000UL)               /*!< 100MHZ                                            */
#define FC_ACCR_80MHZ       ( 80000000UL)               /*!<  80MHZ                                            */
/* FC RACCR setting  configration */
#define FC_RACCR_RAMLC_1CLK (0x00000000UL)              /*!< 1clock(less than 160MHZ).                         */
#define FC_RACCR_RAMLC_2CLK (0x00000010UL)              /*!< 2clock(more than 160MHZ).                         */

/**
 *  @}
 */ /* End of group waitConfiguration */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Private_define HAL Private Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Private_typedef HAL Private Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Private_fuctions HAL Private Fuctions
 *  @{
 */

static uint32_t tick;

/**
 *  @}
 */ /* End of group HAL_Private_functions */

/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup HAL_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief     Increment a tick value.
  * @param     -
  * @retval    -
  * @note      Please call by user.
  * @note      In the sample, this variable is incremented each 1ms timer interrupt.
  */
/*--------------------------------------------------*/
void hal_inc_tick(void)
{
    tick++;
}

/*--------------------------------------------------*/
/**
  * @brief  Provides a tick value.
  * @param  -
  * @return Tick value.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
uint32_t hal_get_tick(void)
{
    return (tick);
}

/*--------------------------------------------------*/
/**
  * @brief  wait Function(us)
  * @param  uint32_t :count(unit:us).
  * @return void
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
void wait(uint32_t count)
{
    uint32_t i, steps;

    /*-----------------------------------------*/
    /* step : 1000000us = fsys :1              */
    /* step = EOSC_SYS_CLOCK/1000000           */
    /*-----------------------------------------*/
    /* system core clock update */
    SystemCoreClockUpdate();
    steps = count * (SystemCoreClock / WAIT_UNIT_1US) / WAIT_FORLOOP_STEPS;

    for (i = 0; i < steps; ++i) {
        __NOP();
    }
}

/*--------------------------------------------------*/
/**
  * @brief  copy 32-bit data from source to dest
  * @param  the address of source and dast, the data size
  * @retval None.
  * @note   -
  */
/*--------------------------------------------------*/
void Copy_Routine(uint32_t *dest, uint32_t *source, uint32_t size)
{
    uint32_t *dest_addr, *source_addr, tmpsize;
    uint32_t i, tmps, tmpd, mask;

    dest_addr = dest;
    source_addr = source;

    tmpsize = size >> 2U;
    for (i = 0U; i < tmpsize; i++) {    /* 32bits copy */
        *dest_addr = *source_addr;
        dest_addr++;
        source_addr++;
    }
    if (size & 0x00000003U) {   /* if the last data size is not 0(maybe 1,2 or 3), copy the last data */
        mask = 0xFFFFFF00U;
        i = size & 0x00000003U;
        tmps = *source_addr;
        tmpd = *dest_addr;
        while (i - 1U) {
            mask = mask << 8U;
            i--;
        }
        tmps = tmps & (~mask);
        tmpd = tmpd & (mask);
        *dest_addr = tmps + tmpd;       /* 32bits copy, but only change the bytes need to be changed */
    } else {
        /* Do nothing */
    }
}

/*--------------------------------------------------*/
/**
  * @brief  RAM register set.
  * @param  none.
  * @retval none.
  * @note   -
  */
/*--------------------------------------------------*/
void fc_ram_con_reg_set(uint32_t sysclock)
{
    uint32_t regval = 0;

    if (sysclock <= FC_ACCR_160MHZ) { /* less than 160MHZ         */
        regval = (uint32_t)FC_RACCR_RAMLC_1CLK;
    } else {                       /* more than 160MHZ         */
        regval = (uint32_t)FC_RACCR_RAMLC_2CLK;
    }

    TSB_FC->KCR = FC_KCR_KEYCODE;
    TSB_FC->RACCR = regval;
    while ((TSB_FC->RACCR != FC_RACCR_RAMLC_1CLK) && (TSB_FC->RACCR != FC_RACCR_RAMLC_2CLK)) {
        /* no processing */
    }
}

/**
 *  @}
 */ /* End of group HAL_Exported_functions */

/**
 *  @}
 */ /* End of group HAL */

/**
 *  @}
 */ /* End of group Periph_Driver */

#endif /* defined(__HAL_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

