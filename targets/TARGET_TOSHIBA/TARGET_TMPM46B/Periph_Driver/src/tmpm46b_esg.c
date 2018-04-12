/**
 *******************************************************************************
 * @file    tmpm46B_esg.c
 * @brief   This file provides API functions for ESG driver.
 * @version V2.0.2.2
 * @date    2018/03/15
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 *******************************************************************************
 */
 
 /* Includes ------------------------------------------------------------------ */
#include "tmpm46b_esg.h"

#if defined(__TMPM46B_ESG_H)
/** @addtogroup TX04_Periph_Driver
  * @{
  */

/** @defgroup ESG 
  * @brief ESG driver modules
  * @{
  */
/** @defgroup ESG_Private_Defines
  * @{
  */
#define ESG_START_SET                 (0x00000001U)
#define ESG_FINTIMING_CLEAR           (0xFFFF0000U)
#define ESG_FINTIMING_SET             (0x0000FFFFU)
#define ESG_LATCHTIMING_SET           (0x000F0000U)
#define ESG_LATCHTIMING_CLEAR         (0xFFF0FFFFU)
#define ESG_BUSY_SET                  (0x00000001U)
#define ESG_INT_SET                   (0x00000001U)
#define SRST_IPRST_3_SET              (0x00000008U)
#define SRST_IPRST_3_CLEAR            (0xFFFFFFF7U)
#define SRST_PROTECT_DISABLE          (0x0000006BU)
#define SRST_PROTECT_ENABLE           (0x00000000U)
#define NUM_BLK                       (16U)

static const volatile uint32_t *const ESG_Blk[NUM_BLK] = {
    &TSB_ESG->BLK00, &TSB_ESG->BLK01, &TSB_ESG->BLK02, &TSB_ESG->BLK03,
    &TSB_ESG->BLK04, &TSB_ESG->BLK05, &TSB_ESG->BLK06, &TSB_ESG->BLK07,
    &TSB_ESG->BLK08, &TSB_ESG->BLK09, &TSB_ESG->BLK10, &TSB_ESG->BLK11,
    &TSB_ESG->BLK12, &TSB_ESG->BLK13, &TSB_ESG->BLK14, &TSB_ESG->BLK15
};

/** @} */
/* End of group ESG_Private_Defines */

/** @defgroup ESG_Private_FunctionPrototypes
    * @{
    */

/** @} */
/* End of group ESG_Private_FunctionPrototypes */

/** @defgroup ESG_Private_Functions
    * @{
    */

/** @} */
/* End of group ESG_Private_Functions */

/** @defgroup ESG_Exported_Functions
  * @{
  */

/** 
  * @brief  Start-up ESG operation.
  * @param  None
  * @retval The value returned can be one of the following values:
  *   SUCCESS or ERROR
  * @register The used registers:
  *   ESGCR<START> 
  */
Result ESG_Startup(void)
{
    Result retval = ERROR;

    if (ESG_GetCalculationStatus() == ESG_CALCULATION_COMPLETE) {
        /* Write '1' to ESGCR<START> to enable  */
        TSB_ESG->CR = ESG_START_SET;
        retval = SUCCESS;
    } else {
        /* Do nothing */
    }

    return retval;
}

/** 
  * @brief  Set entropy seed latch timing.
  * @param  Value: The latch timing for ESG
  *   This parameter can be one of the following values:  
  *   ESG_LATCH_TIMING_1, ESG_LATCH_TIMING_2, ESG_LATCH_TIMING_3,
  *   ESG_LATCH_TIMING_4, ESG_LATCH_TIMING_5, ESG_LATCH_TIMING_6,
  *   ESG_LATCH_TIMING_7, ESG_LATCH_TIMING_8, ESG_LATCH_TIMING_9,
  *   ESG_LATCH_TIMING_10, ESG_LATCH_TIMING_11, ESG_LATCH_TIMING_12,
  *   ESG_LATCH_TIMING_13, ESG_LATCH_TIMING_14, ESG_LATCH_TIMING_15,
  *   ESG_LATCH_TIMING_16.
  * @retval The value returned can be one of the following values:
  *   SUCCESS or ERROR
  * @register The used registers:
  *   ESGOUTCR<LATCHTIMING[19:16]> 
  */
Result ESG_SetLatchTiming(ESG_LatchTiming Value)
{
    /*  Read OUTCR, keep bit 15 to 0, clear bit 19 to 16 */
    uint32_t tmp = TSB_ESG->OUTCR & ESG_LATCHTIMING_CLEAR;
    Result retval = ERROR;

    /* Check the parameters */
    assert_param(IS_ESG_LATCH_TIMING(Value));

    if (ESG_GetCalculationStatus() == ESG_CALCULATION_COMPLETE) {
        tmp |= (uint32_t) Value << 16;
        TSB_ESG->OUTCR = tmp;
        retval = SUCCESS;
    } else {
        /* Do nothing */
    }

    return retval;
}

/** 
  * @brief  Get entropy seed latch timing.
  * @param  None
  * @retval tmp: the value of entropy seed latch timing
  * @register The used registers:
  *   ESGOUTCR<LATCHTIMING[19:16]> 
  */
uint32_t ESG_GetLatchTiming(void)
{
    uint32_t tmp;

    tmp = (TSB_ESG->OUTCR & ESG_LATCHTIMING_SET) >> 16;

    return tmp;
}

/** 
  * @brief  Set Entropy seed output timing.
  * @param  Fintming: the value of entropy seed output timing
  * @retval The value returned can be one of the following values:
  *   SUCCESS or ERROR
  * @register The used registers:
  *   ESGOUTCR<FINTIMING[15:0]> 
  */
Result ESG_SetFintiming(uint16_t Fintming)
{
    /* Read OUTCR, keep bit 19 to 16, clear bit 15 to 0 */
    uint32_t tmp = TSB_ESG->OUTCR & ESG_FINTIMING_CLEAR;
    Result retval = ERROR;

    /* Get latchtiming value */
    uint16_t latchtiming = (uint16_t) ESG_GetLatchTiming();

    if (ESG_GetCalculationStatus() == ESG_CALCULATION_COMPLETE) {
        if (Fintming >= 512U * (latchtiming + 1U) + 3U) {
            tmp |= (uint32_t) Fintming;
            TSB_ESG->OUTCR = tmp;
            retval = SUCCESS;
        } else {
            /* Do nothing */
        }
    } else {
        /* Do nothing */
    }

    return retval;
}

/** 
  * @brief  Get entropy seed  Fintiming.
  * @param  None
  * @retval tmp: the value of entropy seed Fintiming
  * @register The used registers:
  *   ESGOUTCR<FINTIMING[15:0]> 
  */
uint16_t ESG_GetFintiming(void)
{
    uint16_t tmp;

    tmp = (uint16_t) TSB_ESG->OUTCR & ESG_FINTIMING_SET;

    return tmp;
}

/** 
  * @brief  Clear the ESG interrupt.
  * @param  None
  * @retval The value returned can be one of the following values:
  *   SUCCESS or ERROR
  * @register The used registers:
  *   ESGINT<INT> 
  */
Result ESG_ClrInt(void)
{
    Result retval = ERROR;

    if (ESG_GetCalculationStatus() == ESG_CALCULATION_COMPLETE) {
        /* Write '1' to ESGINT<INT> to clear the interrupt */
        TSB_ESG->INT = ESG_INT_SET;
        retval = SUCCESS;
    } else {
        /* Do nothing */
    }

    return retval;
}

/** 
  * @brief  Get the ESG interrupt status.
  * @param  None
  * @retval The value returned can be one of the following values:
  *   ENABLE: interrupt occurs 
  *   DISABLE: no interrupt
  * @register The used registers:
  *   ESGINT<INT> 
  */
FunctionalState ESG_GetIntStatus(void)
{
    FunctionalState retval = DISABLE;

    if (TSB_ESG->INT == ESG_INT_SET) {
        retval = ENABLE;
    } else {
        /* Do nothing */
    }

    return retval;
}

/**
  * @brief  Reset ESG by peripheral function.
  * @param  None
  * @retval None
  * @register The used register:
  *   SRSTIPRST<IPRST3>
  *   SRSTPROTECT
  */
void ESG_IPReset(void)
{
    uint32_t iprst3 = 0U;

    /* Disable write protection state of SRSTIPRST */
    TSB_SRST->PROTECT = SRST_PROTECT_DISABLE;

    TSB_SRST->IPRST |= SRST_IPRST_3_SET;

    /* Release reset state */
    TSB_SRST->IPRST &= SRST_IPRST_3_CLEAR;

    /* Enable write protection state of SRSTIPRST */
    TSB_SRST->PROTECT = SRST_PROTECT_ENABLE;
}

/**
  * @brief  Get the calculation status.
  * @param  None
  * @retval The calculation status.
  *   The value returned can be one of the following values:
  *   ESG_CALCULATION_COMPLETE, ESG_CALCULATION_PROCESS.
  * @note  Do not write any value to ESG registers when calculation is in process.
  * @register The used register:
  *   ESGST<BUSY>
  */
ESG_CalculationStatus ESG_GetCalculationStatus(void)
{
    ESG_CalculationStatus retval = ESG_CALCULATION_COMPLETE;

    if (TSB_ESG->ST == ESG_BUSY_SET) {
        retval = ESG_CALCULATION_PROCESS;
    } else {
        /* Do nothing */
    }

    return retval;
}

/** 
  * @brief  Get the calculation result.
  * @param  Seed[16U]: A point that points to the value of calculation result.
  * @retval None  
  * @register The used registers:
  *   ESGBLK00 to ESGBLK15
  */
void ESG_GetResult(uint32_t Seed[NUM_BLK])
{
    uint32_t i = 0U;

    for (i = 0U; i < NUM_BLK; i++) {
        Seed[i] = *ESG_Blk[i];
    }
}

/** @} */
/* End of group ESG_Exported_Functions */

/** @} */
/* End of group ESG */

/** @} */
/* End of group TX04_Periph_Driver */
#endif                          /* defined(__TMPM46B_ESG_H) */
